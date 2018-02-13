#ifndef __FASTCGI_H__
#define __FASTCGI_H__

#include <tcp.h>
#include <type.h>
#include <buffer.h>

enum fcgi_request_type {
  FCGI_BEGIN_REQUEST = 1,
  FCGI_ABORT_REQUEST = 2,
  FCGI_END_REQUEST = 3,
  FCGI_PARAMS = 4,
  FCGI_STDIN = 5,
  FCGI_STDOUT = 6,
  FCGI_STDERR = 7,
  FCGI_DATA = 8,
  FCGI_GET_VALUES = 9,
  FCGI_GET_VALUES_RESULT = 10,
  FCGI_UNKOWN_TYPE = 11
};

enum fcgi_role {
  FCGI_RESPONDER = 1,
  FCGI_AUTHORIZER = 2,
  FCGI_FILTER = 3
};


struct FCGI_Header {
  uchar version;
  uchar type;
  uchar requestIdB1;
  uchar requestIdB0;
  uchar contentLengthB1;
  uchar contentLengthB0;
  uchar paddingLength;
  uchar reserved;
};

enum begin_request_flags {
  FCGI_KEEP_CONN = 1
};

struct FCGI_BeginRequestBody {
  uchar roleB1;
  uchar roleB0;
  uchar flags; //whether to close the connection
  uchar reserved[5];
};

enum fcgi_protocol_status {
  FCGI_REQUEST_COMPLETE = 0,  
  FCGI_CANT_MPX_CONN = 1,  
  FCGI_OVERLOADED = 2,  
  FCGI_UNKNOWN_ROLE = 3  
};

struct FCGI_EndRequestBody {  
  uchar appStatusB3;  
  uchar appStatusB2;  
  uchar appStatusB1;  
  uchar appStatusB0;  
  uchar protocolStatus;  
  uchar reserved[3];  
};  

class Fastcgi {
private:
  Speaker speaker;
  char *response_stdout;
  char *response_stderr;

  void send_request_header(uchar type, ushort requestId, ushort contentLength);
  void send_content(uchar type, ushort requestId, const Buffer &buffer);
  void send_begin_request(ushort requestId, ushort role, uchar flag);
  void send_params(ushort requestId, const params_type &params);
  void send_stdin(ushort requestId, const char *s);

  FCGI_Header recv_header();
public:
  Fastcgi(const char *ip, int proxy = 9000);
  ~Fastcgi();
  void send(ushort requestId, const params_type &params, const char *s);

  void recv();
  char *get_stdout() const;
  char *get_stderr() const;
} ;
#endif

