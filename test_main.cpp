#include <iostream>
#include <thread>
#include <chrono>
#include "work_item.hpp"
#include "queue_handler.hpp"
using namespace std;

void read_items(int mode, QueueHandler<shared_ptr<WorkItem> > &q) {
  if (mode == 0) {
      cout << "using pop" << endl;
      while (true) {
          auto work_itm = q.pop();
          cout << "Reading: " << *work_itm << endl;
      }
  } else {
      while (true) {
          try{
              auto work_itm = q.pop_try();
              cout << "Reading: " << *work_itm << endl;
          } catch (int e) {
              cout << "No item in queue, exiting thread" << endl;
              break;
          }
      }
  }
}


void insert_items(QueueHandler<shared_ptr<WorkItem> > &q) {
  while (true) {
      int type = rand() % 3;
      shared_ptr<WorkItem> sp(new WorkItem(type));
      cout << "Inserting " << *sp << endl;
      try {
          q.push(sp);
      } catch (int e) {
          cout << "Error: could not push item" << endl;
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

    QueueHandler<shared_ptr<WorkItem> > q{};

    // Add some items to the queue initially
    for (int i = 0; i < 3; ++i) {
        shared_ptr<WorkItem> sp(new WorkItem);
        q.push(sp);
    }

    thread reader(read_items, mode, ref(q));

    insert_items(q);

    reader.join();

    return 0;
};
