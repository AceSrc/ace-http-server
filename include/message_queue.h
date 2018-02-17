/* header file: MESSAGE_QUEUE_H */
#ifndef __MESSAGE_QUEUE_H_
#define __MESSAGE_QUEUE_H_

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class Queue {
  typedef std::unique_ptr<T> uptr;

private:
  std::queue<uptr> q;
  std::mutex m;
  std::condition_variable cv;
  int cnt;
  bool is_stopped;

public:
  Queue () : is_stopped(false) {
  }

  ~Queue () {
  }

  uptr pop() {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [&]{ return !q.empty() || is_stopped; });
    if (q.empty() && is_stopped) {
      return nullptr;
    }

    auto rt = std::move(q.front());
    q.pop();
    return rt;
  }  

  bool push(uptr x) {
    if (is_stopped) return false;
    std::unique_lock<std::mutex> lock(m);
    q.push(std::move(x));
    lock.unlock();
    cv.notify_one();
    return true;
  }

  void stop() {
    std::unique_lock<std::mutex> lock(m);
    lock.unlock();
    is_stopped = true;
  }

  void start() {
    std::unique_lock<std::mutex> lock(m);
    lock.unlock();
    is_stopped = false;
  }
} ;

#endif /* end of include guard: MESSAGE_QUEUE_H */
