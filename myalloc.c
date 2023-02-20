#include "myalloc.h"

struct block *head = NULL;

/**
 *  Allocate a given number of bytes & return a pointer to it.
 */
void *myalloc(int size)
{
    int padded_block_size = PADDED_SIZE(sizeof(struct block));

    if (head == NULL)
    {
        head = mmap(
            NULL, 1024, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
        head->next = NULL;
        head->size = 1024 - padded_block_size;
        head->in_use = 0;
    }

    struct block *b = head;

    while (b != NULL)
    {
        bool is_free = (b->in_use == 0);
        bool has_enough_room = (b->size >= PADDED_SIZE(size));

        if (is_free && has_enough_room)
        {
            b->in_use = 1;
            return PTR_OFFSET(b, 0);
        }
        b = b->next;
    }
    return NULL;
}

/**
 * Free allocated memory of a given pointer.
 */
void myfree(void *p)
{
    return;
}

/**
 * Print allocated memory address space.
 */
void print_data(void)
{
    struct block *b = head;

    if (b == NULL)
    {
        printf("[empty]\n");
        return;
    }

    while (b != NULL)
    {
        printf("[%d,%s]", b->size, b->in_use ? "used" : "free");
        if (b->next != NULL)
            printf(" -> ");
        b = b->next;
    }
    printf("\n");
}

/**
 * Program entrypoint.
 */
int main(void)
{
    void *p;

    print_data();
    p = myalloc(16);
    print_data();
    p = myalloc(16);
    printf("%p\n", p);

    return 0;
}
