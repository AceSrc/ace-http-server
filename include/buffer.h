#ifndef __BUFFER_H__
#define __BUFFER_H__

class Buffer {
private:
  char *buf;
  char *p;
  int cpos;
  int buf_size;
public:
  Buffer(); 
  ~Buffer();

  void clear();
  void append(const char *s, int wrlen = -1);
  char *str(int offset = 0) const;
  int size() const;
} ;

#endif
