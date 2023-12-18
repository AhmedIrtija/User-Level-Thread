# User-Level-Thread

This project, written in C, creates threads using semaphores, preemption, and queues to run processes concurrently.

## How the Code Works

1. **Download MinGW to Run GCU**
    - Download MinGW [here](https://sourceforge.net/projects/mingw/).

2. **Add MinGW Bin Folder to Your PATH**
   
3. **Change Directory to /apps**
   
4. **Compile the Code**
    - Execute the following command in the terminal:
      ```
      make
      ```

5. **Run Files to Test**
    - Execute the following files in the terminal to test specific functionalities:
        - ```./preempt_test``` - Test Preemption
        - ```./sem_buffer``` - Test semaphores using buffers
        - ```./sem_count```  - Test semaphores by counting
        - ```./sem_prime```  - Test semaphores using prime numbers
        - ```./sem_simple```  - Test semaphores using a very simple logic
        - ```./uthread_hello```  - Test User Level Threading
        - ```./uthread_yield```  - Test yield in User Level Threading
        - ```./thread_testing``` - Test all attributes of all files
