#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include <mutex>
#include "work_item.hpp"
#include "queue_handler.hpp"
using namespace std;

static mutex mtx;

void read_items(int mode, QueueHandler<shared_ptr<WorkItem> > &q) {
  if (mode == 0) {
      while (true) {
          auto work_itm = q.pop();
          mtx.lock();
          cout << "Reading: " << *work_itm << endl;
          mtx.unlock();
      }
  } else {
      while (true) {
          try{
              auto work_itm = q.pop_try();
              mtx.lock();
              cout << "Reading: " << *work_itm << endl;
              mtx.unlock();
          } catch (int e) {
              mtx.lock();
              cout << "No item in queue, exiting thread" << endl;
              mtx.unlock();
              break;
          }
      }
  }
}


void insert_items(QueueHandler<shared_ptr<WorkItem> > &q) {
  while (true) {
      int type = rand() % 3;
      shared_ptr<WorkItem> sp(new WorkItem(type));
      mtx.lock();
      cout << "Inserting " << *sp << endl;
      mtx.unlock();
      try {
          q.push(sp);
      } catch (int e) {
          mtx.lock();
          cout << "Error: could not push item" << endl;
          mtx.unlock();
          break;
      }
      // Add sleep to simulate a writer thread that pushes on events
      this_thread::sleep_for(chrono::seconds(2));
  }
}

void exit_msg() {
    cerr << "Incorrect arguments, use pop or pop_try" << endl;
    exit(1);
}

int main(int argc, char **argv) {
    if (argc != 2) {
      exit_msg();
    }

    int mode;
    if (strcmp(argv[1], "pop") == 0) {
        mode = 0;
    } else if (strcmp(argv[1], "pop_try") == 0) {
        mode = 1;
    } else {
        exit_msg();
    }

    QueueHandler<shared_ptr<WorkItem> > q(8);

    // Add some items to the queue initially
    for (int i = 0; i < 3; ++i) {
        shared_ptr<WorkItem> sp(new WorkItem(i));
        cout << "Inserting " << *sp << endl;
        q.push(sp);
    }

    thread reader(read_items, mode, ref(q));

    insert_items(q);

    reader.join();

    return 0;
};
