#ifndef __TOOLS_H__
#define __TOOLS_H__

template<typename T>
T min(const T &a, const T &b) {
  if (a < b) return a;
  return b;
}
#endif
