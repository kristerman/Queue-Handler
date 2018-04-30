#ifndef __QUEUE_HANDLER_HPP
#define __QUEUE_HANDLER_HPP

#include <queue>
#include <utility>
#include <mutex>
#include <condition_variable>

template<class T>
class QueueHandler {
    public:
      QueueHandler(uint max_queue_size) {
        m_max_queue_size = max_queue_size;
      }

      T pop() {
        std::unique_lock<std::mutex> lk(m_mtx);
        while (this->m_queue.empty()) {
          m_cv.wait(lk);
        }
        T next = std::move(this->m_queue.front());
        this->m_queue.pop();
        lk.unlock();
        return next;
      }

      T pop_try() {
        std::lock_guard<std::mutex> lk(m_mtx);
        if (!this->m_queue.empty()) {
          T next = std::move(this->m_queue.front());
          this->m_queue.pop();
          return next;
        } else {
          throw -1;
        }
      }

      void push(const T &WorkItem) {
          {
            std::lock_guard<std::mutex> lk(m_mtx);
            if (this->m_queue.size() == this->m_max_queue_size) {
                throw -1;
            }
            this->m_queue.push(move(WorkItem));
          }
          m_cv.notify_one();
      }

    private:
      std::queue<T> m_queue;
      uint m_max_queue_size = 6;
      std::mutex m_mtx;
      std::condition_variable m_cv;
  };

#endif
