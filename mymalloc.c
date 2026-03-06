#include <stdio.h>
#include <stddef.h>
#include "mymalloc.h"
//This is the main implemenation file
//Only allowed to use heap.bytes
#define MEMLENGTH 4096

static union { //The union forces 8 byte alignment
    char bytes[MEMLENGTH];
    double not_used;
} heap;

//declaration of heap.bytes
typedef struct Header{
    size_t size; //payload size, including the header
    int is_free; //1=free, 0=allocated
} Header;

static int intialized = 0; //Only one static int

size_t round_up(size_t size){ //Round up to a multiple oof 8
    return (size + 7) & ~7;
}

int is_allocated(Header *h){ //Get the allocation status
    return (*h).size & 1;
}

int get_size(Header *h){ //return size of the chunk
    return (*h).size & ~1;
}

void allocated_mark(Header *h){ //return if the space is taken
    (*h).size |= 1;
}

void free_mark(Header *h){ //return if the space is free
    (*h).size |= ~1;
}
void leak_detector(){ // will detect if there is any memory that's not released in the heap
    Header *current = (Header *)heap.bytes; //current pointer in the memory
    int count = 0;
    size_t total = 0;

    while ((char *)current < heap.bytes + MEMLENGTH){  //traverse the heap
        if (is_allocated(current)) { //checks if the pointer to current chuck is allocated with space
            count++;
            total += get_size(current) - sizeof(Header); 
            //the total is the differenece between the size of the allocated space and the size of the header
        }
        current = (Header *)((char *)current + get_size(current)); //the new current is the pointer to the next block
    }
    if(count > 0){
        fprintf(stderr, "mymalloc: %zu bytes leaked in %d objects. \n", total, count);
        //output the amount of unreleased memory in bytes
    }
}
//intialize the function and create the a large free chunk of memory
void initializeheap(){
    Header *first = (Header *)heap.bytes;
    (*first).size = MEMLENGTH; //whole heap free
    intialized = 1;
    atexit(leak_detector);//automatically search the chunk for leaks of memory
}

static void mycoalesce(){
    Header *current = (Header *)heap.bytes; //create the pointer
    while((char *)current < heap.bytes + MEMLENGTH){ //traverse the heap
        Header *next = (Header *)((char *)current + sizeof(Header) + (*current).size); //create the poniter to the next header
        if((char *)next < heap.bytes + MEMLENGTH && (*current).is_free && (*next).is_free){//if the current and next header is free
            (*current).size += sizeof(Header) + (*next).size;//merge the two together
            continue;
        }
        current = next; //go to the next header
    }
}

//mymalloc implementation
void *mymalloc(size_t size, char *file, int line){
    if(size <= 0) return NULL;

    if (!intialized){ //if the heap is not created
        initializeheap(); //create a new and free heap
    }
    size = round_up(size);
    size_t totalsize = size + sizeof(Header);

    Header *current = (Header *)heap.bytes; //current pointer

    while((char *)current < heap.bytes + MEMLENGTH){ //traverse the heap
        if(!is_allocated(current) && get_size(current) >= totalsize) {
            if((*current).size >= size + sizeof(Header) + 8) { //split the chunk if the space is greater or equal to a new header and 8 byte
                Header *nextchunk = (Header *)((char *)current + sizeof(Header));//Create a new Header for the next chunk
                (*nextchunk).size = (*current).size - size - sizeof(Header);
                (*nextchunk).is_free = 1;
                (*current).size;
            }
            allocated_mark(current); //mark that the space has been allocated
            return (void *)((char *)current + sizeof(Header)); //return payload pointer
        }
        current = (Header *)((char *)current + sizeof(Header) + (*current).size);
    }
    fprintf(stderr, "mymalloc: Unable to allocate %zu bytes leaked in %s:%d\n", size, file, line);
    return NULL;
}

//myfree() implementation
void myfree(void *pointer, char *file, int line){
    if (pointer == NULL) return;
    //create error detection for calling myfree() without an address from mymalloc()
    if((char *)pointer < heap.bytes || (char *)pointer >= heap.bytes + MEMLENGTH){
        //Create error detection, check if the current pointer is within heap range
        fprintf(stderr, "myfree: Pointer %p not in heap. %s:%d\n", pointer, file, line);
        return;
    }
    Header *h = (Header *)((char *)pointer - sizeof(Header));
    Header *find = (Header *)heap.bytes;//error detection: pointer at the start of a payload
    //valid payload
    int valid = 0;
    while ((char *)find < heap.bytes + MEMLENGTH){
        if (find == h){
            valid = 1;
            break;
        }
        find = (Header *)((char *)find + sizeof(Header) + (*find).size);
    }
    //create error detectiom if the address is not at the start of the chunk
    if(!valid){
        fprintf(stderr, "myfree: Pointer %p is not at the start of the chunk. %s:%d\n");
        return;
    }
    //create error detection myfree() is called twice on the same pointer
    if((*h).is_free){
        fprintf(stderr, "myfree: myfree() called a second time on pointer %p in %s:%d\n", pointer, file, line);
    }
    (*h).is_free = 1;
    mycoalesce();
}