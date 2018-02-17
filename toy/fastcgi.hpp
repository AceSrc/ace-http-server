/* header file: FASTCGI_HPP */
#ifndef __FASTCGI_HPP_
#define __FASTCGI_HPP_



#define HEAD_LEN                8  //消息头长度固定为8个字节
#define BUFLEN                  4096
#define FCGI_VERSION_1           1  //版本号

enum fcgi_request_type {
    FCGI_BEGIN_REQUEST      = 1,
    FCGI_ABORT_REQUEST      = 2,
    FCGI_END_REQUEST        = 3,
    FCGI_PARAMS             = 4,
    FCGI_STDIN              = 5,
    FCGI_STDOUT             = 6,
    FCGI_STDERR             = 7,
    FCGI_DATA               = 8,
    FCGI_GET_VALUES         = 9,
    FCGI_GET_VALUES_RESULT  = 10,
    FCGI_UNKOWN_TYPE        = 11
};

enum fcgi_role {
    FCGI_RESPONDER      = 1,
    FCGI_AUTHORIZER     = 2,
    FCGI_FILTER         = 3
};

struct fcgi_header {
    unsigned char version;
    unsigned char type;
    unsigned char requestIdB1;
    unsigned char requestIdB0;
    unsigned char contentLengthB1;
    unsigned char contentLengthB0;
    unsigned char paddingLength;
    unsigned char reserved;
};

struct FCGI_BeginRequestBody {
    unsigned char roleB1;
    unsigned char roleB0;
    unsigned char flags;
    unsigned char reserved[5];
};

struct FCGI_EndRequestBody {
    unsigned char appStatusB3;
    unsigned char appStatusB2;
    unsigned char appStatusB1;
    unsigned char appStatusB0;
    unsigned char protocolStatus;
    unsigned char reserved[3];
};

enum protocolStatus {
    FCGI_REQUEST_COMPLETE = 0,
    FCGI_CANT_MPX_CONN = 1,
    FCGI_OVERLOADED = 2,
    FCGI_UNKNOWN_ROLE = 3
};

#endif /* end of include guard: FASTCGI_HPP */
