#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

typedef struct node //To tranverse through the queue
{
	void *data;
	struct node *next;
	struct node *prev;
} node;

struct queue
{ 
	// the data structure for running the queue
	int length;
	node *begin;
	node *end;
};

queue_t queue_create(void) 
{
	//Malloc the size and initialize the values
	queue_t q = malloc(sizeof(queue_t));
	if(!q){
		return NULL;
	}
	q->length = 0;
	q->begin = NULL;
	q->end = NULL;
	return q;
}

int queue_destroy(queue_t queue)
{
	//Free memory
	if(queue == NULL || (queue->length != 0))
	{
		return -1;
	}
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	// Check if NULL
	if (queue == NULL || data == NULL){
		return -1;
	}
	// Create new data node
	node *newItem = malloc(sizeof(node));
	// Handle Malloc error
	if (!newItem)
	{
		return -1;
	}
	newItem->data = data;
	newItem->prev = NULL;
	if (!queue->length)
	{
		newItem->next = NULL;
		queue->begin = newItem;
		queue->end = newItem;
		queue->length++;
		return 0;
	}
	else
	{
		newItem->next = queue->begin;
		queue->begin->prev = newItem;
		queue->begin = newItem;
		queue->length++;
		return 0;
	}
}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue && queue->length > 0 && data) {
		//Set size of node holder
		node *qHolder;
		*data = queue->end->data;
		qHolder = queue->end;
		queue->end->next = NULL;
		//Store the data then move to the next
		queue->end = queue->end->prev;
		//Change size and deallocate memory
		queue->length--;
		free(qHolder);
		return 0;
	} else{
		return -1;
	}
}

void remove_node(queue_t q, node *newItem)
{
	// Beginning case
	if ((newItem->prev == NULL) && (newItem->next != NULL))
	{
		newItem->next->prev = NULL;
		q->begin = newItem->next;
		free(newItem);
		q->length -= 1;
	}
	// End case
	else if ((newItem->next == NULL) && (newItem->prev != NULL))
	{
		newItem->prev->next = NULL;
		q->end = newItem->prev;
		free(newItem);
		q->length -= 1;
	}
	// One item case
	else if ((newItem->next == NULL) && (newItem->prev == NULL))
	{
		q->end = NULL;
		q->begin = NULL;
		q->length -= 1;
		free(newItem);
	}
	else
	{
		// Middle case
		newItem->next->prev = newItem->prev;
		newItem->prev->next = newItem->next;
		free(newItem);
		q->length -= 1;
	}
}

int queue_delete(queue_t queue, void *data)
{
	// NULL and Empty checks
	if (!queue || !data || !queue->length)
	{
		return -1;
	}
	// Iterate through queue until data or end is found
	node *head = queue->end;
	while (head->data != data)
	{
		if (head->prev == NULL)
		{
			return -1;
		}
		head = head->prev;
	}
	// Remove data node
	remove_node(queue, head);	
	return 0;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if(queue == NULL || func == NULL){
		return -1;
	} else {
		//Set the node to the oldest value
		node *iterate = queue->end;
		while(iterate != NULL){
			//run the function
			func(queue, iterate->data);
			iterate = iterate->prev;
		}
	}
	return 0;
}

int queue_length(queue_t queue)
{
	if(queue == NULL){
		return -1;
	}
	return queue->length;
}



