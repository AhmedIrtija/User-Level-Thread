#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore { 
	size_t sCount; 
	queue_t blockedQueue;
};

sem_t sem_create(size_t count)
{
	//malloc sem then initialize it's items
	sem_t oneSem = malloc(sizeof(sem_t));
	if(oneSem == NULL){
		return NULL;
	}
	//Create blocked queue
	oneSem->blockedQueue = queue_create();
	if(oneSem->blockedQueue == NULL){
		free(oneSem);
		return NULL;
	}
	//set count
	oneSem->sCount = count;

	return oneSem;

}

int sem_destroy(sem_t sem)
{
	//Check for error 
	if(sem == NULL || queue_length(sem->blockedQueue) != 0){
		return -1;
	}
	//destroy and free memory
	queue_destroy(sem->blockedQueue);
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	if(!sem){
		return -1;
	}

	//preempt enable or disable here 
	
	//Enqueue the current thread to blocking state then call uthread block
	if(sem->sCount == 0){
		if(queue_enqueue(sem->blockedQueue, uthread_current()) == -1){
			return -1;
		} else{
			uthread_block();
		}
	}
	sem->sCount--;
	return 0;
}

int sem_up(sem_t sem)
{
	//preempt enable or disable here 
	if(!sem){
		return -1;
	}
	sem->sCount++;
	//dequeue the blocked queue then unblock it
	if(queue_length(sem->blockedQueue) > 0){
		struct uthread_tcb *unblocked = NULL;
		if(queue_dequeue(sem->blockedQueue, (void**)&unblocked) == -1){
			return -1;
		}

		uthread_unblock(unblocked);
	}
	return 0;
}

