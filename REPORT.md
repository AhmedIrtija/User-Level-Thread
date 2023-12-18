# ECS150: Project #2 - User-level thread library
____Ahmed Irtija____

The aim of this project was to implement User-Level Thread

## Phase 1: queue API
In this phase we implemented a simple FIFO queue using a doubly linked list.
We chose this data structure, as it is a dynamically allocated data structure 
and does not impose any capacity limitations like an array implementation 
might.
Additionally, linked lists have no memory wastage, and meet the **O(1)** 
runtime requirements for most operations.

We defined two structs, queue and node, representing the entire linked list 
queue and each single data node in the linked list respectively. Queue simply
stores the beginning, end, and length of the queue, while each node stores 
its data, and the previous and next nodes.

Creating a queue simply initialzes a queue struct with length 0 and null front
and back pointers, while destroy frees an empty queues allocated memory.

Enqueue takes non-null data, allocates a new node, and fits the node into the
beginning of the linked list by setting both the queue's begin pointer and the
most recently enqueued node's previous pointer to the new node. Dequeue removes
the last node in the linked list by setting the queue's end to the second to
last node, setting the new last node's next pointer to null, and freeing the
dequeued node.

Deleting a specific data node takes two steps: finding the node and then 
removing it. The delete function loops through the next node pointers until 
it finds the first instance of the node it is searching for, and then calls 
remove node to remove it. Remove node takes care of the restructuring of the 
linked list, by freeing the allocated node memory, and setting its neighbor's
next and previous pointers to each other, restoring the link.

Iterate starts at the oldest/last data node in the queue, applies the given
function to its data, and then moves to the previous node and so on. And so it
iterates till it reaches the beginning/most recent item of the queue.

## Phase 2: uthread API
In this phase, we implemented a simple thread management API, which allows
applications to use multithreading through an interface.

We use two queues defined in Phase 1, one storing ready threads and the other
storing zombie threads. We define the different states of a thread using an 
enum object to keep track of the state at which the threads are in. 
Additionally, we define a TCB struct, which stores the context, stack pointer,
and state of a certain thread. We keep track of 3 essential threads: the idle
thread, the current thread, and the previous thread.

Starting the multithreading library with u_thread_run does a few things. It
creates the ready and zombie queues, creates the idle thread, sets the current
thread to the idle thread, and sets the idle thread state to runnning. It then
creates the first thread to be run, and starts the main while loop. This loop
handles garbage collection each iteration, by iterating over the zombie queue
and frees the zombie threads. If there are still threads to be run in the ready
queue after each thread is done executing, it yields for the next thread. And 
so it sequentially runs each thread until all have completed and exited. It 
then finishes by freeing the two queues and the idle thread.

Creating a thread allocates the memory for its TCB, initialzes the thread state
to Ready, and allocates a stack. After allocation error handling, the thread in
enqueued in the Ready queue. 

Exiting a given thread at the end of its execution sets the previous thread to
it, changes its state to zombie, adds it to the zombie queue, and sets the next
thread from the ready queue to be the new running current thread. It then
switches context to the new current thread.

Yielding a thread simply updates the previous thread to the current one, sets
the new previous thread to ready and enqueues it back into the ready queue to
run later. It then sets the new current thread to running and switches
contexts.

## Phase 3: semaphore API
This phase added on to the previous two, implementing semaphores to manage
resource allocation for our multithreading library.

We implement semaphores using a struct which keeps track of the available
resource count, and a queue containing all blocked threads waiting for the
resource.

Creating a semaphore simply allocates memory for the object, sets the given
resource count, and creates the blocked queue. Destroying a semaphore does
exactly the opposite, by freeing the blocked queue and the memory allocated to
itself.

Taking a resource with sem_down blocks the current thread if there are no
available resources (resource count is 0), and will simply decrease the
resource count by 1 otherwise. Releasing a resource with sem_up increases the
resource count by 1, and unblocks the next thread in the blocked queue if it
exists (blocked queue is not empty).

Blocking a thread consists of setting the previous thread variable to the
current thread, setting its state to blocked, getting and setting the new
current thread's state to running, and switching contexts. Unblocking does the
opposite by setting the given thread's state to ready and adding it to the
ready queue to be run.

## Phase 4: preemption
In this last phase, we implemented the preemption feature, which solves the
problem of threads hogging resources till they are finished executing by using
timer interrupts.

We defined a signal set which keeps track of which signals are being used and
are blocked/unblocked, and a preemption timer.

Starting preemption firstly sets the action to take when a signal is received
to call the signal handler. It then configures the timer to have an interrupt
interval/frequency of 100 times per second, and starts it immediately.
Stopping preemption simply removes the action to take when the alarm signal is
received.

Disabling preemption simply updates the signal set to block the alarm signal.
Enabling it does the opposite by updating the signal set to unblock the alarm
signal.

Our signal handler is only called when preemption is enabled and when it
receives an timer interrupt signal, and yields the current thread to the next.

## Testing
### Phase 1
We tested our implementation with 28 unit tests we decided on. These tests
cover error handling and functionality, and all passed. Additionally, we
tested for memory leaks using Valgrind, and found 79 allocs and 79 frees.

### Phase 2
We tested our implementation with the given tests, and additionally found no
memory leaks using Valgrind.

### Phase 3
As in the previous phase, we tested our semaphore implementation by using the
given tests and Valgrind, and found no errors or memory leaks.

### Phase 4
We wrote our own test file for preemption and also used Valgrind to check for
memory leaks.

### Sources
We started with the skeleton code provided, and primarily used code blocks and
examples from lecture slides, C documentation, and the project description as
sources.

### Conclusion
In this project, we implemented a user level multithreading library to be used
by applications through the interface. Some of the features we included include
semaphores for managing resources and preemption to prevent resource hogging.
