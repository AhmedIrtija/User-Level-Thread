#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../libuthread/queue.h"

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

//Test create
void test_create(void)
{
	fprintf(stderr, "\n*** TEST create ***\n");
	queue_t q = queue_create();
	TEST_ASSERT(q != NULL);
	queue_destroy(q);
}

// Enqueue Test
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "\n*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
	queue_destroy(q);
}

// Destroy NULL queue
void test_destroy_null_queue(void)
{
	queue_t q = NULL;
	fprintf(stderr, "\n*** TEST destroy_null_queue ***\n");
	TEST_ASSERT(queue_destroy(q) == -1);
}
// Destroy with empty queue
void test_destroy_empty_queue(void)
{
	queue_t q;
	fprintf(stderr, "\n*** TEST destroy_empty_queue ***\n");
	q = queue_create();
	TEST_ASSERT(queue_destroy(q) == 0);
}
// Destroy with full queue
void test_destroy_filled_queue(void)
{
	queue_t q;
	int data = 3, *ptr;
	fprintf(stderr, "\n*** TEST destroy_filled_queue ***\n");
	q = queue_create();
	queue_enqueue(q, &data);
	TEST_ASSERT(queue_destroy(q) == -1);
	queue_dequeue(q, (void**)&ptr);
	queue_destroy(q);
}
// Enqueue NULL data
void test_enqueue_null_data(void){
	queue_t q;
	q = queue_create();
	char* data = NULL;
	fprintf(stderr, "\n*** TEST test_enqueue_null_data ***\n");
	TEST_ASSERT(queue_enqueue(q, data) == -1);
	queue_destroy(q);
}
//Enqueue NULL queue
void test_enqueue_null_queue(void){
	queue_t q = NULL;
	int data = 3;
	fprintf(stderr, "\n*** TEST test_enqueue_null_queue ***\n");
	TEST_ASSERT(queue_enqueue(q, &data) == -1);
}
// Enqueue one string
void test_enqueue_string(void)
{
	queue_t q;
	char* data = "TEST STRING", *ptr;
	fprintf(stderr, "\n*** TEST void test_enqueue_string(void) ***\n");
	q = queue_create();
	TEST_ASSERT(queue_enqueue(q, data) == 0);
	queue_dequeue(q, (void**)&ptr);
	queue_destroy(q);
}
// Enqueue one int
void test_enqueue_int(void)
{
	queue_t q;
	int data = 5, *ptr;
	fprintf(stderr, "\n*** TEST void test_enqueue_int(void) ***\n");
	q = queue_create();
	TEST_ASSERT(queue_enqueue(q, &data) == 0);
	queue_dequeue(q, (void**)&ptr);
	queue_destroy(q);
}
// Enqueue one struct
void test_enqueue_struct(void)
{
	typedef struct test_obj
	{
		void *data;
	} test_obj;
	queue_t q;
	void *d;
	test_obj obj;
	fprintf(stderr, "\n*** TEST void test_enqueue_struct(void) ***\n");
	q = queue_create();
	TEST_ASSERT(queue_enqueue(q, &obj) == 0);
	queue_dequeue(q, (void**)&d);
	queue_destroy(q);
}
// Enqueue multiple items with different types
void test_enqueue_multiple(void)
{
	queue_t q;
	int data1 = 5, *ptr1;
	int data2 = 6, *ptr2;
	int data3 = 7, *ptr3;
	fprintf(stderr, "\n*** TEST void test_enqueue_multiple(void) ***\n");
	q = queue_create();
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	queue_dequeue(q, (void**)&ptr1);
	queue_dequeue(q, (void**)&ptr2);
	queue_dequeue(q, (void**)&ptr3);
	queue_destroy(q);
	TEST_ASSERT((ptr1 == &data1) && (ptr2 == &data2) && (ptr3 == &data3));
}
//Dequeue one item
void test_dequeue_one(void){
	queue_t q;
	int data = 5, *ptr;
	fprintf(stderr, "\n*** TEST void test_dequeue_one(void) ***\n");
	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	queue_destroy(q);
	TEST_ASSERT(ptr == &data);
}
//Dequeue empty queue
void test_dequeue_empty(void){
	queue_t q;
	int *ptr;
	q = queue_create();
	fprintf(stderr, "\n*** TEST void test_dequeue_empty(void) ***\n");
	TEST_ASSERT(queue_dequeue(q, (void**)&ptr) == -1);
	queue_destroy(q);
}
//Dequeue multiple items
void test_dequeue_multiple(void){
	queue_t q;
	int data1 = 5, *ptr1;
	int data2 = 6, *ptr2;
	int data3 = 7, *ptr3;
	fprintf(stderr, "\n*** TEST void test_dequeue_multiple(void) ***\n");
	q = queue_create();
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	queue_dequeue(q, (void**)&ptr1);
	queue_dequeue(q, (void**)&ptr2);
	queue_dequeue(q, (void**)&ptr3);
	queue_destroy(q);
	TEST_ASSERT((ptr1 == &data1) && (ptr2 == &data2) && (ptr3 == &data3));
}
//Dequeue NULL data
void test_dequeue_null_data(void){
	queue_t q;
	char** data = NULL;
	q = queue_create();
	fprintf(stderr, "\n*** TEST void test_dequeue_null_data(void) ***\n");
	TEST_ASSERT(queue_dequeue(q, (void**)&data) == -1);
	queue_destroy(q);
}
//Dequeue NULL queue
void test_dequeue_null_queue(void){
	queue_t q;
	char** data;
	q = NULL;
	fprintf(stderr, "\n*** TEST void test_dequeue_null_queue(void) ***\n");
	TEST_ASSERT(queue_dequeue(q, (void**)&data) == -1);
}
//Delete NULL data
void test_delete_null_data(void){
	queue_t q; 
	q = queue_create();
	int data1 = 5, *ptr1; 
	int data2 = 7, *ptr2;
	char* data3 = NULL;
	fprintf(stderr, "\n*** TEST void test_delete_null_data(void) ***\n");
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	TEST_ASSERT(queue_delete(q, (void**)&data3) == -1);
	queue_dequeue(q, (void**)&ptr1);
	queue_dequeue(q, (void**)&ptr2);
	queue_destroy(q);
}
//Delete NULL queue
void test_delete_null_queue(void){
	queue_t q; 
	q = NULL;
	int data = 7;
	fprintf(stderr, "\n*** TEST void test_delete_null_queue(void) ***\n");
	TEST_ASSERT(queue_delete(q, (void**)&data) == -1);
}
//Delete when item doesn't exist
void test_delete_not_found(void){
	queue_t q; 
	q = queue_create();
	int data = 8, *ptr;
	int data1 = 9;
	fprintf(stderr, "\n*** TEST void test_delete_not_found(void) ***\n");
	queue_enqueue(q, &data);
	TEST_ASSERT(queue_delete(q, &data1) == -1);
	queue_dequeue(q, (void**)&ptr);
	queue_destroy(q);
}
//Delete first item
void test_delete_first(void){
	queue_t q;
	q = queue_create();
	int data = 8, data1 = 9, *ptr;
	fprintf(stderr, "\n*** TEST void test_delete_first(void) ***\n");
	queue_enqueue(q, &data);
	queue_enqueue(q, &data1);
	queue_delete(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT( *ptr == 9);
	queue_destroy(q);
}
//Delete middle item
void test_delete_middle(void){
	queue_t q;
	q = queue_create();
	int data = 8, *ptr1;
	int data1 = 9;
	int data2 = 45, *ptr2;
	fprintf(stderr, "\n*** TEST void test_delete_middle(void) ***\n");
	queue_enqueue(q, &data);
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_delete(q, &data1);
	queue_dequeue(q, (void**)&ptr1);
	queue_dequeue(q, (void**)&ptr2);
	TEST_ASSERT( *ptr1 == 8 && *ptr2 == 45);
	queue_destroy(q);
}
//Delete last item
void test_delete_last(void){
	queue_t q;
	q = queue_create();
	int data = 8, *ptr1;
	int data1 = 9, *ptr2;
	int data2 = 45;
	fprintf(stderr, "\n*** TEST void test_delete_last(void) ***\n");
	queue_enqueue(q, &data);
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_delete(q, &data2);
	queue_dequeue(q, (void**)&ptr1);
	queue_dequeue(q, (void**)&ptr2);
	TEST_ASSERT(*ptr1 == 8 && *ptr2 == 9);
	queue_destroy(q);
}
//Delete first instance
void test_delete_first_item(void){
	queue_t q;
	q = queue_create();
	int data1 = 8, *ptr1, data2 = 9, *ptr2, data3 = 4, data4 = 9, *ptr3;
	fprintf(stderr, "\n*** TEST void test_delete_first_item(void) ***\n");
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	queue_enqueue(q, &data4);
	queue_delete(q, &data2);
	queue_dequeue(q, (void**)&ptr1);
	queue_dequeue(q, (void**)&ptr2);
	queue_dequeue(q, (void**)&ptr3);
	TEST_ASSERT(*ptr1 == 8 && *ptr2 == 4 && *ptr3 == 9);
	queue_destroy(q);
}

//Function to test null
void iterating_test_null(queue_t queue, void *data){
	queue_t iterate = malloc(sizeof(queue_t));
	iterate = queue;
	void *ch = data;
	printf("%d\t%p\n", queue_length(iterate), ch);
	free(iterate);
}
//Iterates null queue
void test_iterate_null_queue(void){
	queue_t q = NULL;
	fprintf(stderr, "\n*** TEST void test_iterate_null_queue(void) ***\n");
	TEST_ASSERT(queue_iterate(q, iterating_test_null) == -1);
}
//Iterates null function
void test_iterate_null_func(void){
	queue_t q = queue_create();
	queue_func_t iterate_test_func = NULL;
	int data = 5, *ptr1, data1 = 6, *ptr2, data2 = 7, *ptr3;
	fprintf(stderr, "\n*** TEST void test_iterate_null_func(void) ***\n");
	queue_enqueue(q, &data);
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	TEST_ASSERT(queue_iterate(q, iterate_test_func) == -1);
	queue_dequeue(q, (void**)&ptr1);
	queue_dequeue(q, (void**)&ptr2);
	queue_dequeue(q, (void**)&ptr3);
	queue_destroy(q);
}
//Function to test iterate 
void iterate_test_math(queue_t queue, void* data){
	int *doub = (int*) data;
	int ch = *doub;
	printf("\n%d\t%d\n", queue_length(queue), ch);
}
//Iterates correctly and runs function
void test_iterate(void){
	queue_t q = queue_create();
	int data1 = 1, *ptr1;
	int data2 = 2, *ptr2;
	int data3 = 3, *ptr3;
	int data4 = 4, *ptr4;
	int data5 = 5, *ptr5;
	int data6 = 6, *ptr6;
	int data7 =  7, *ptr7;
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	queue_enqueue(q, &data4);
	queue_enqueue(q, &data5);
	queue_enqueue(q, &data6);
	queue_enqueue(q, &data7);
	fprintf(stderr, "\n*** TEST void test_iterate(void) ***\n");
	int check = queue_iterate(q, iterate_test_math);
	TEST_ASSERT(check == 0);
	queue_dequeue(q, (void**)&ptr1);
	queue_dequeue(q, (void**)&ptr2);
	queue_dequeue(q, (void**)&ptr3);
	queue_dequeue(q, (void**)&ptr4);
	queue_dequeue(q, (void**)&ptr5);
	queue_dequeue(q, (void**)&ptr6);
	queue_dequeue(q, (void**)&ptr7);
	queue_destroy(q);
}
//Test length
void test_length(void){
	queue_t q = queue_create();
	int data1 = 1, *ptr1;
	int data2 = 2, *ptr2;
	int data3 = 3, *ptr3;
	int data4 = 4, *ptr4;
	int data5 = 5, *ptr5;
	int data6 = 6, *ptr6;
	int data7 =  7, *ptr7;
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	queue_enqueue(q, &data4);
	queue_enqueue(q, &data5);
	queue_enqueue(q, &data6);
	queue_enqueue(q, &data7);
	fprintf(stderr, "\n*** TEST void test_length(void) ***\n");
	TEST_ASSERT(queue_length(q) == 7);
	queue_dequeue(q, (void**)&ptr1);
	queue_dequeue(q, (void**)&ptr2);
	queue_dequeue(q, (void**)&ptr3);
	queue_dequeue(q, (void**)&ptr4);
	queue_dequeue(q, (void**)&ptr5);
	queue_dequeue(q, (void**)&ptr6);
	queue_dequeue(q, (void**)&ptr7);
	queue_destroy(q);

}
//Test length when its empty
void test_length_empty(void){
	queue_t q = queue_create();
	fprintf(stderr, "\n*** TEST void test_length_empty(void) ***\n");
	TEST_ASSERT(queue_length(q) == 0);
	queue_destroy(q);
}
//Example Function provided by the Prof
static void iterator_inc(queue_t q, void *data)
{
	int *a = (int*)data;

	if (*a == 42){
		queue_delete(q, data);
	} else{
		*a += 1;
	}
}
//Example test provided by the Prof
void test_iterator(void)
{
	queue_t q;
	int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
	int *ptr;
	size_t i;

	/* Initialize the queue and enqueue items */
	q = queue_create();
	for (i = 0; i < sizeof(data) / sizeof(data[0]); i++){
		queue_enqueue(q, &data[i]);
	}
	/* Increment every item of the queue, delete item '42' */
	queue_iterate(q, iterator_inc);
	fprintf(stderr, "\n*** TEST void test_iterator(void) ***\n");
	TEST_ASSERT(data[0] == 2);
	TEST_ASSERT(queue_length(q) == 9);
	for (i = 0; i < (sizeof(data) / sizeof(data[0])) - 1; i++){
		queue_dequeue(q, (void**)&ptr);
	}
	queue_destroy(q);
}

int main(void)
{
	test_create();
	test_queue_simple();
	test_destroy_null_queue();
	test_destroy_empty_queue();
	test_destroy_filled_queue();
	test_enqueue_null_data();
	test_enqueue_null_queue();
	test_enqueue_string();
	test_enqueue_int();
	test_enqueue_struct();
	test_enqueue_multiple();
	test_dequeue_one();
	test_dequeue_empty();
	test_dequeue_multiple();
	test_dequeue_null_data();
	test_dequeue_null_queue();
	test_delete_null_data();
	test_delete_null_queue();
	test_delete_not_found();
	test_delete_first();
	test_delete_middle();
	test_delete_last();
	test_iterate_null_queue();
	test_iterate_null_func();
	test_iterate();
	test_length();
	test_length_empty();
	test_iterator();
	test_delete_first_item();
	return 0;
}
