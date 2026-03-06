#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

int main(){
    void *a1 = malloc(100);
    void *a2 = malloc(100);
    void *a3 = malloc(100);

    free(a1);
    free(a2);
    void *a4 = malloc(180);
    if(a4 == a1) {
        printf("Coalescing works!!\n");
    }
    else{
        printf("Coalescing does not work :(");
    }

    free(a3);
    free(a4);
    return 0;
}