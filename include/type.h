#ifndef __TYPE_H__
#define __TYPE_H__

#include <map>
#include <string.h>
#include <message_queue.h>
#include <buffer.h>
typedef const char * key_type;
typedef const char * value_type;


class CMP_KEY {
public:
  bool operator() (const key_type &lhs, const key_type &rhs) const {
    return strcmp(lhs, rhs) < 0;
  }
} ;

typedef std::map<key_type, value_type, CMP_KEY> params_type;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef Queue<Buffer> message_queue_type;
#endif
