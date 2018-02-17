#include <tcp.h>
#include <type.h>
#include <fastcgi.h>
#include <cstdio>

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

static std::string int_to_str(int x, int bits) {
  char buf[4];
  if (bits == 1) {
    buf[0] = x; 
    return std::string(buf, 1);
  }
  else {
    *((unsigned int *)buf) = transform(x | 0x80000000); 
    return std::string(buf, 4);
  }
}

static std::string calc_param_len(const std::string &s) {
  /* TODO: check the document && check the correctness */
  int size = s.size();
  if (size <= PARAM_LEN_THRESHOLD) return int_to_str(size, 1);
  return int_to_str(size, 4);
}

void Fastcgi::send_content(uchar type, ushort requestId, const std::string &buffer) {
  int sent_len = 0;
  int buffer_size = buffer.size();
  while (sent_len < buffer_size) {
    int current_sent_len = buffer_size - sent_len;
    if (MAX_CONTENT_LENGTH < current_sent_len) current_sent_len = MAX_CONTENT_LENGTH;

    send_request_header(type, requestId, current_sent_len);
    speaker.send(buffer.substr(sent_len, current_sent_len), current_sent_len);
    sent_len += current_sent_len;
  }
  send_request_header(type, requestId, 0);
}

void Fastcgi::send_params(ushort requestId, const params_type &params) {
  std::string buffer;
  for (auto i = params.begin(); i != params.end(); ++i) {
    buffer += calc_param_len(i->first);
    buffer += calc_param_len(i->second);
    buffer += i->first;
    buffer += i->second;
  }

  //printf("%s\n", buffer.str());
  send_content(FCGI_PARAMS, requestId, buffer);
}

void Fastcgi::send_stdin(ushort requestId, const std::string &s) {
  send_content(FCGI_STDIN, requestId, s);
}

FCGI_Header Fastcgi::recv_header() {
  return speaker.recv_struct<FCGI_Header>();
}

static int calc_contentLength(const FCGI_Header &header) {
  return (header.contentLengthB1 << MASK_LENGTH) | header.contentLengthB0;
}

void Fastcgi::recv() {
  response_stdout = "";
  response_stderr = ""; 
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
      response_stdout += speaker.recv(calc_contentLength(header));
    }
    else if (header.type == FCGI_STDERR) 
      response_stderr += speaker.recv(calc_contentLength(header));
    else {
      printf("Invaild type %d\n", header.type);
      exit(1);
    }
    speaker.recv(header.paddingLength);
  }
}

void Fastcgi::send(ushort requestId, const params_type &params, const std::string &s) {
  send_begin_request(requestId, FCGI_RESPONDER, !FCGI_KEEP_CONN);
  send_params(requestId, params);
  send_stdin(requestId, s);
}

Fastcgi::Fastcgi(const std::string &ip, int proxy) : response_stdout(""), response_stderr("") {
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(ip.c_str());
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
}

std::string Fastcgi::get_stdout() const {
  return response_stdout;
}

std::string Fastcgi::get_stderr() const {
  return response_stderr;
}

