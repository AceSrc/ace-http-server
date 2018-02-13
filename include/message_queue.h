/* header file: MESSAGE_QUEUE_H */
#ifndef __MESSAGE_QUEUE_H_
#define __MESSAGE_QUEUE_H_

#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>

template<typename T>
class Queue {
  typedef std::unique_ptr<T> uptr;

private:
  std::queue< uptr > q;
  std::mutex m;
  std::condition_variable cv;
  //std::unique_lock<std::mutex> lock;
  //std::function<void(const uptr &)> handler;
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
    std::cout << "pop" << std::endl;
    if (is_stopped) {
      //cout << "Stopped" << endl;
      is_stopped = false;
      return nullptr;
    }

    auto rt = std::move(q.front()); 
    //std::cout << rt.get()->id << std::endl;
    q.pop();
    //handler(std::move(rt));
    return rt;
  }  

  void push(uptr x) {
    std::unique_lock<std::mutex> lock(m);
    //std::cout << "Push" << std::endl;
    //cv.wait(lock);

    //x.get()->id = ++cnt;
    q.push(std::move(x));
    //std::cout << "Push " << cnt << std::endl;

    lock.unlock();
    cv.notify_one();
  }

  void once() {
    std::unique_lock<std::mutex> lock(m);
    lock.unlock();
    is_stopped = true;
    cv.notify_one();
  }
} ;

#endif /* end of include guard: MESSAGE_QUEUE_H */
