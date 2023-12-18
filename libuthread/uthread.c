#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "queue.h"
#include "uthread.h"

//Initialize global queues and enum
queue_t readyQueue = NULL;
queue_t zombieQueue = NULL;
typedef enum PStates {Ready, Running, Blocked, Zombie} PStates;

typedef struct uthread_tcb {
	uthread_ctx_t context;
	void* stack;
	PStates state;
} tcb;

//Global threads to handle context switching 
tcb *IDLE = NULL;
tcb *currentThread = NULL;
tcb *prevThread = NULL;

tcb *uthread_current(void)
{
	return currentThread;
}

void uthread_yield(void)
{
	//Sets the previous thread to the current thread
	prevThread = currentThread;
	prevThread->state = Ready;
	//Sets to ready before putting it back in ready queue
	queue_enqueue(readyQueue, prevThread);
	//receive the next thread
	queue_dequeue(readyQueue, (void**)&currentThread);
	currentThread->state = Running;
	//set to running then context switch to run it
	uthread_ctx_switch(&(prevThread->context), &(currentThread->context));
}

void uthread_exit(void)
{
	prevThread = currentThread;
	prevThread->state = Zombie;
	//Set to zombie then put it in zombie queue
	queue_enqueue(zombieQueue, prevThread);
	//Receive the current thread to run
	queue_dequeue(readyQueue, (void**)&currentThread);

	currentThread->state = Running;
	//Switch from zombie thread to running thread
	uthread_ctx_switch(&(prevThread->context), &(currentThread->context));
}

int uthread_create(uthread_func_t func, void *arg)
{
	tcb *oneThread = malloc(sizeof(tcb));
	//initialize tcb and check error
	if(oneThread == NULL){
		return -1;
	}

	oneThread->state = Ready;
	oneThread->stack = uthread_ctx_alloc_stack();

	if(oneThread->stack == NULL){
		free(oneThread);
		return -1;
	}
	// error check then set context
	if(uthread_ctx_init(&(oneThread->context), (oneThread->stack), func, arg) == -1){
		uthread_ctx_destroy_stack(oneThread->stack);
		free(oneThread);
		return -1;
	}
	
	// perfect initialization then enqueue in ready queue
	if(queue_enqueue(readyQueue, oneThread) == -1){
		uthread_ctx_destroy_stack(oneThread->stack);
		free(oneThread);
		return -1;
	}
	
	return 0;
}

void deleteZombie(){
	//Dequeue the zombie and clear the memory
	tcb *Zholder = NULL;
	queue_dequeue(zombieQueue, (void**)&Zholder);
	uthread_ctx_destroy_stack(Zholder->stack);
	free(Zholder);
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	//Create the ready and zombie queue
	readyQueue = queue_create();
	if(readyQueue == NULL){
		return -1;
	}

	zombieQueue = queue_create();
	if(zombieQueue == NULL){
		queue_destroy(readyQueue);
		return -1;
	}
	//Create the Idle thread 
	if(uthread_create(NULL, NULL) == -1){
		queue_destroy(readyQueue);
		queue_destroy(zombieQueue);
		return -1;
  	}
	//Initialize the Idle thread and set it to running
	queue_dequeue(readyQueue, (void**)&IDLE);
	currentThread = IDLE;
	currentThread->state = Running;

	//Create the first thread if not free memory
	if(uthread_create(func, arg) == -1){
		queue_destroy(readyQueue);
		queue_destroy(zombieQueue);
		uthread_ctx_destroy_stack(IDLE->stack);
		free(IDLE);
		return -1;
	}

	preempt_start(preempt);

	//Run infinite loop and yield to the first thread 
	while(1){
		queue_iterate(zombieQueue, deleteZombie);
		if(queue_length(readyQueue)){
			uthread_yield();
		} else {
			break;
		}
	}

	if(preempt){
		preempt_stop();
	}
  
	//free all mallocs 
	queue_destroy(zombieQueue);
  	queue_destroy(readyQueue);
	uthread_ctx_destroy_stack(IDLE->stack);
  	free(IDLE);

	return 0;
}

void uthread_block(void)
{
	//Set previous thread to blocked then context switch to new thread
	prevThread = currentThread;
	prevThread->state = Blocked;
	queue_dequeue(readyQueue, (void**)&currentThread);
	currentThread->state = Running;
	uthread_ctx_switch(&(prevThread->context), &(currentThread->context));
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	//Set the given thread to ready then enqueue it
	uthread->state = Ready;
	queue_enqueue(readyQueue, uthread);
}