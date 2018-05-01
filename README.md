## Queue-Handler

The main component of this project is a template class that implements
a FIFO queue of Work Items, intended to be used by a high priority writer thread and
a background reader thread. 

The exposed API of this class consists of methods: pop, pop_try and push.
Push is intended to be called by the writer thread, whereas pop and pop_try
are intended to be called by the reader thread.

push method adds items to the queue until the max size of the queue is reached
where the method throws -1, therefore it should be executed in an appropriate try/catch
block.

pop gets the next available item, if there is none available the thread is put
into sleep until an item is added to the queue.

pop_try is a non-blocking call that attempts to get the next available item in the queue. 
If there is no item method throws -1 and thus reader thread should call pop_try in a try/catch block too. 

## Test class

To demonstrate how this class will work, a test WorkItem class has been created as well as
a main uses the 2 threads mentioned in a way that could simulate a realistic use.

Compile by: g++ -std=c++11 work_item.cpp test_main.cpp -pthread -o queue-test
The executable can be run by: ./queue-test [pop|pop_try]
Depending on which method the reader thread will be using.

Note: When run using pop, program must be terminated by user (Ctrl + C).


