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
  std::unique_lock<std::mutex> lock;
  int cnt;

public:
  Queue () {
  }

  ~Queue () {

  }

  uptr pop() {
    std::unique_lock<std::mutex> lock(m);
    cv.wait(lock, [&]{ return !q.empty(); });

    auto rt = std::move(q.front()); 
    q.pop();
    std::cout << "Pop " << rt.get()->id << std::endl;

    //cv.notify_one();
    return rt;
  }  

  void push(uptr x) {
    std::unique_lock<std::mutex> lock(m);
    //std::cout << "Push" << std::endl;
    //cv.wait(lock);

    x.get()->id = ++cnt;
    q.push(std::move(x));
    std::cout << "Push " << cnt << std::endl;

    lock.unlock();
    cv.notify_one();
  }
} ;

#endif /* end of include guard: MESSAGE_QUEUE_H */
