#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"
//tests the free()
int main(){
    void *p1 = malloc(200);
    free(p1);

    void *p2 = malloc(200);

    if(p1 == p2){
        printf("Free() works correctly.\n");
    }
    else{
        printf("Free() is not marking free blocks correctly.\n");
    }
    free(p2);
    return 0;
}