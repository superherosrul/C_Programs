#include <stdlib.h>
#include "mymalloc.h"

int main(){
    int x;
    free(&x); //should return an invalid pointer
    return;
}