#ifndef __TYPE_H__
#define __TYPE_H__

#include <unordered_map>
#include <string>

#include <message_queue.h>

typedef std::string key_type;
typedef std::string value_type;

typedef std::unordered_map<key_type, value_type> params_type;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef Queue<std::string> message_queue_type;
typedef std::unique_ptr<std::string> uptr;
#endif
