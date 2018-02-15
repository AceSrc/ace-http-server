#include <tcp.h>
#include <type.h>
#include <fastcgi.h>
#include <buffer.h>
#include <cstdio>
#include <tools.h>

/* TODO: need test */

#define FCGI_VERSION 0x01
#define MASK_LENGTH 8
#define MASK 0xff
#define ZERO_BYTE 0x00
#define MAX_CONTENT_LENGTH 65535
#define PARAM_LEN_THRESHOLD 127

void Fastcgi::send_request_header(uchar type, ushort requestId, ushort contentLength) {
  FCGI_Header header; 
  header.version = FCGI_VERSION;
  header.type = type; 
  header.requestIdB1 = requestId >> MASK_LENGTH;
  header.requestIdB0 = requestId & MASK;
  header.contentLengthB1 = contentLength >> MASK_LENGTH;
  header.contentLengthB0 = contentLength & MASK;
  header.paddingLength = ZERO_BYTE;
  header.reserved = ZERO_BYTE;
  speaker.send_struct<FCGI_Header>(header);
}

void Fastcgi::send_begin_request(ushort requestId, ushort role, uchar flag) {
  FCGI_BeginRequestBody body;
  body.roleB1 = role >> MASK_LENGTH;
  body.roleB0 = role & MASK;
  body.flags = flag;
  send_request_header(FCGI_BEGIN_REQUEST, requestId, (ushort)sizeof(body));
  speaker.send_struct<FCGI_BeginRequestBody>(body);
}

static uint transform(uint x) {
  return ((x & 0xff) << 24) | ((x & 0xff00) << 16) | ((x & 0xff0000) >> 16) | ((x & 0xff000000) >> 24);
}

static const char *int_to_str(int x, int bits, char *buf, Buffer &buffer) {
  if (bits == 1) {
    buf[0] = x; 
    buf[1] = '\0'; 
    buffer.append(buf, 1);
  }
  else {
    *((unsigned int *)buf) = transform(x | 0x80000000); 
    buf[4] = '\0'; 
    buffer.append(buf, 4);
  }
  //printf("int_to_str: %s(0x%08x)\n", buf, *((unsigned int *)buf));
  return buf;
}

static const char *calc_param_len(const char *s, char *buf, Buffer &buffer) {
  /* TODO: check the document && check the correctness */
  int size = strlen(s);
  if (size <= PARAM_LEN_THRESHOLD) return int_to_str(size, 1, buf, buffer);
  return int_to_str(size, 4, buf, buffer);
}

void Fastcgi::send_content(uchar type, ushort requestId, const Buffer &buffer) {
  //printf("Sending content\n");
  int sent_len = 0;
  while (sent_len < buffer.size()) {
    int current_sent_len = buffer.size() - sent_len;
    if (MAX_CONTENT_LENGTH < current_sent_len) current_sent_len = MAX_CONTENT_LENGTH;
    send_request_header(type, requestId, current_sent_len);
    speaker.send(buffer.str(sent_len), current_sent_len);
    sent_len += current_sent_len;
    //printf("Sendt %d bytes(%d)\n", sent_len, current_sent_len);
  }
  send_request_header(type, requestId, 0);
}

//#include <iostream>
//using namespace std;
void Fastcgi::send_params(ushort requestId, const params_type &params) {
  Buffer buffer;
  char len_buf[5];
  for (auto i = params.begin(); i != params.end(); ++i) {
    calc_param_len(i->first, len_buf, buffer);
    calc_param_len(i->second, len_buf, buffer);
    buffer.append(i->first);
    buffer.append(i->second);
    //cout << i->first << '=' << i->second << endl;
  }

  //char *p = buffer.str();
  //printf("%d---\n", buffer.size());
  //for (int i = 0; i < buffer.size(); i++) {
    ////printf("H");
    //printf("[%u] ", (unsigned char)p[i]);
  //}

  printf("%s\n", buffer.str());
  //cout << buffer.str() << endl;
  send_content(FCGI_PARAMS, requestId, buffer);
}

void Fastcgi::send_stdin(ushort requestId, const char *s) {
  Buffer buffer;
  buffer.append(s);
  send_content(FCGI_STDIN, requestId, buffer);
}

FCGI_Header Fastcgi::recv_header() {
  return speaker.recv_struct<FCGI_Header>();
}

static void copy_to(char *&p, const char *s) {
  int size = strlen(s);
  if (p) free(p);
  p = (char *)malloc(size + 1);
  memcpy(p, s, size + 1);
}

static int calc_contentLength(const FCGI_Header &header) {
  return (header.contentLengthB1 << MASK_LENGTH) | header.contentLengthB0;
}

void Fastcgi::recv() {
  Buffer stdout_buffer;
  Buffer stderr_buffer;
  while (true) {
    auto header = recv_header(); 
    printf("version: %d type: %d\n length: 0x%02x%02x(%d) paddinglength: 0x%02x\n", header.version, header.type, header.contentLengthB1, header.contentLengthB0, calc_contentLength(header), header.paddingLength);
    if (header.type == FCGI_END_REQUEST) {
      speaker.recv_struct<FCGI_EndRequestBody>();
      break;
    }
    if (calc_contentLength(header) == 0) {
      continue;
    }
    if (header.type == FCGI_STDOUT) {
      stdout_buffer.append(speaker.recv(calc_contentLength(header)));
      //cout << stdout_buffer.size() << ' ' << stdout_buffer.str() << endl;
    }
    else if (header.type == FCGI_STDERR) 
      stderr_buffer.append(speaker.recv(calc_contentLength(header)));
    else {
      printf("Invaild type %d\n", header.type);
      exit(1);
    }
    speaker.recv(header.paddingLength);
  }
  copy_to(response_stdout, stdout_buffer.str());
  copy_to(response_stderr, stderr_buffer.str());
  //printf("%s\n", response_stdout);
  //printf("%s\n", response_stderr);

}

void Fastcgi::send(ushort requestId, const params_type &params, const char *s) {
  send_begin_request(requestId, FCGI_RESPONDER, !FCGI_KEEP_CONN);
  send_params(requestId, params);
  if (s != NULL) send_stdin(requestId, s);
}

Fastcgi::Fastcgi(const char *ip, int proxy) : response_stdout(nullptr), response_stderr(nullptr) {
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip);
  addr.sin_port = htons(proxy);

  int clientfd = socket(PF_INET, SOCK_STREAM, 0);
  //printf("%s %d\n", ip, proxy);
  if (connect(clientfd, (sockaddr *)&addr, sizeof(sockaddr)) == -1) {
    printf("Can't connect to(%d)\n", errno);
    printf("%s\n", strerror(errno));
    exit(1);
  }
  speaker.setfd(clientfd);
  speaker.setwfd(clientfd);
}

Fastcgi::~Fastcgi() {
  //~speaker();
}

char *Fastcgi::get_stdout() const {
  //printf("%s\n", response_stdout);
  return response_stdout;
}

char *Fastcgi::get_stderr() const {
  return response_stderr;
}
