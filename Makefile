#Programs
TESTS = overlaptest freetest coalescetest errortest leaktest

all: memgrind $(TESTS)

#Compile memgrind
memgrind: memgrind.c mymalloc.c
    gcc -Wall -Wextra -std=c11 memgrind.c mymalloc.c -o memgrind

#Each test program compiles
overlaptest: overlaptest.c mymalloc.c
    gcc -Wall -Wextra -std=c11 overlaptest.c mymalloc.c -o overlaptest

freetest: freetest.c mymalloc.c
    gcc -Wall -Wextra -std=c11 freetest.c mymalloc.c -o freetest

coalescetest: coalescetest.c mymalloc.c
    gcc -Wall -Wextra -std=c11 coalescetest.c mymalloc.c -o coalescetest

errortest: errortest.c mymalloc.c
    gcc -Wall -Wextra -std=c11 errortest.c mymalloc.c -o errortest

leaktest: leaktest.c mymalloc.c
    gcc -Wall -Wextra -std=c11 leaktest.c mymalloc.c -o leaktest

clean:
    rm -f memgrind $(TESTS)
