#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <memory>
#include <thread>
#include <chrono>
#include "work_item.hpp"
using namespace std;


template<class T>
class QueueHandler {
    public:
      QueueHandler() {}
      ~QueueHandler() {}

      T pop() {
        if (!this->m_queue.empty()) {
          T next = move(this->m_queue.front());
          this->m_queue.pop();
          return next;
        } else {
          throw "Error: empty queue";
        }
      }

      void push(const T &WorkItem) {
        if (this->m_queue.size() == this->m_max_queue_size) {
          throw "Error: Cannot add WorkItem";
        }
        this->m_queue.push(move(WorkItem));
      }

    private:
      queue<T> m_queue;
      uint m_max_queue_size = 6;
  };

void read_items(QueueHandler<shared_ptr<WorkItem> > &q) {
    using namespace chrono_literals;
    while (true) {
      try{
        auto work_itm = q.pop();
        cout << "Reading: " << *work_itm << endl;
      } catch (const char* e) {
        cout << e << endl;
        cout << "reader going to sleep" << endl;
        this_thread::sleep_for(2s);
        cout << "sleep is over" << endl;
      }
    }
}

template<class T>
void insert_items(QueueHandler<shared_ptr<T> > &q) {
  while (true) {
    shared_ptr<T> sp(new T);
    try {
        q.push(sp);
    } catch (const char* e) {
      cout << e << endl;
      break;
    }
  }
  cout << "finished inserting" << endl;
}


int main() {
  QueueHandler<shared_ptr<WorkItem> > q{};
  //read_items(q);
  thread reader(read_items, ref(q));


  insert_items(q);

  reader.join();

  return 0;
};
