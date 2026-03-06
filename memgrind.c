//stress testing
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "mymalloc.h"

//malloc() and immeadiately free() a 1-byte object 120 times
void workload1(){
    for(int x; x < 120; x++){
        void *p = malloc(1); //allocate 1-byte object
        free(p); //free that object
    }
}
//use malloc() to get 120 1-byte objects, storing the pointers in an array
//then use free() to deallocate the chunks
void workload2(){
    void *pointers[120]; //array
    for(int x; x < 120; x++){
        pointers[x] = malloc(1); //store the allocate 1-byte object to the array
    }
    for(int x; x < 120; x++){
        free(pointers[x]); //free each of the objects after allall objects were allocated and stored in the array
    }
}

//Create an array of 120 pointers. Repeatedly make a random choice between (a) allocating a
//1-byte object and adding the pointer to the array and (b) deallocating a random previously
//allocated object (if any). Once 120 allocations have been performed, deallocate all remaining
//objects.
void workload3(){
    void *pointers[120]; //array of pointers
    int allocated = 0; //to keep track of allocated objects
    int total_allocated = 0; //keep track of the 120 allocations

    while(total_allocated < 120){
        int rchoice = rand() % 2; //%2 will always output a 0 or 1

        if(rchoice == 0 && total_allocated < 120){ //if the the random choice is 0 and there is space for allocation
            //(a) allocate 1-byte and add pointer to the array
            pointers[allocated] = malloc(1);
            allocated++;
            total_allocated++;
        }
        else if(allocated > 0){ //if the random choice is 1
            //(b) if there are any objects to be deallocated
            int x = rand() % allocated; //pick a random pointers that is currently allocated
            free(pointers[x]);
            pointers[x] - pointers[allocated - 1];
            allocated--;
        }
    }
    for(int x; x < allocated; x++){
        free(pointers[x]);
    }
}

void workload4(){
    void *pointers[20];

    for(int x = 0; x < 5; x++){
        pointers[x] = x + 1;       
    }
    for(int x = 0; x < 5; x++){
        printf("%d ", pointers[x]);
    }
    for(int x = 0; x < 5; x++){
        free(pointers[x]);
    }
}
void workload5(){
    char *words = malloc(20);
    free(words);
    words = malloc(strlen("Hello, World!") + 1);
    strcpy(words, "Hello, World!");
    printf("%s\n",words);

    if(words != NULL){
        free(words);
        words = NULL;
    }
}
int main(){
    struct timeval start, end;
    gettimeofday(&start, NULL);

    for(int x = 0; x < 50; x++){
        workload1();
        workload2();
        workload3();
        workload4();
        workload5();
    }

    gettimeofday(&end, NULL);

    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;

    double elapsed = seconds + microseconds * 1e-6;
    double average = elapsed / 50;

    printf("Average time per run: %f seconds\n", average);

    return 0;
}