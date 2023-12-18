#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include "private.h"

int check = 0;

void thread2(void* arg){
    (void) arg;
    printf("Reached Thread2\n");
    //Preemption worked if it reaches here
    check = 1;
}

void thread1(void* arg){
    (void) arg;
    uthread_create(thread2, NULL);
    //Create an infinite loop for thread1 to keep working
    while(1){
        printf("Waiting in Thread1\n");
        if(check == 1) break;
    }
}

int main(void){

    uthread_run(true, thread1, NULL);
    printf("Done\n");

    return 0;
}