#include <sys/mman.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define ALIGNMENT 16 // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x)-1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void *)((char *)(p) + (offset)))

struct block
{
    struct block *next;
    int size;   // bytes
    int in_use; // boolean
};

void *myalloc(int size);
void myfree(void *p);
void print_data(void);
int main(void);