#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mymalloc.h"
//wil test if the overlap detection works
int main(){
    //make up some random objects to take up space
    char *a = malloc(100);
    char *a2 = malloc(100);

    memset(a, 'A', 100);
    memset(a2, 'A2', 100);

    for(int x = 0; x < 100; x++){
        if(a[x]){
            printf("Memory overlap found!\n");
            return 1;
        }
    }
    printf("No overlap found.\n");

    free(a);
    free(a2);
    return 0;
}