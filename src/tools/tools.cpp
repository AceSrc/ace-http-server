#include <tools.h>
char *bufdup(const char *s, int size) {
  if (!s) return nullptr;
  if (size == -1) size = strlen(s);
  size++; 

  char *rt = (char *)malloc(size);
  memcpy(rt, s, size);
  *(rt + size - 1) = '\0';
  return rt;
}

