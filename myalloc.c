#include "myalloc.h"

struct block *head = NULL;

/**
 *  Allocate a given number of bytes & return a pointer to it.
 */
void *myalloc(int size)
{
    if (head == NULL)
    {
        head = mmap(
            NULL, 1024, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
        head->size = 1024 - PADDED_SIZE(sizeof(struct block));
        head->next = NULL;
        head->in_use = 0;
    }

    struct block *b = head;

    while (b != NULL)
    {
        bool is_free = !b->in_use;
        bool has_enough_room = (b->size >= size);

        if (is_free && has_enough_room)
        {
            b->in_use = 1;
            b->size = PADDED_SIZE(size);
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
    (void)p; // silence is golden
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
    void *p = myalloc(117);
    print_data();
    printf("%p", p);

    return 0;
}
