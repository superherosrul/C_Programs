#include <stdlib.h>
#include "mymalloc.h"

int main(){
    malloc(100);
    malloc(200); 
    //should return with a leak detection error
    return 0;
}