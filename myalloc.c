#include "myalloc.h"

struct block *head = NULL;

/**
 *  Allocate a given number of bytes & return a pointer to it.
 */
void *myalloc(int size)
{
    int padded_block_size = PADDED_SIZE(sizeof(struct block));

    // initialize the heap
    if (head == NULL)
    {
        head = mmap(
            NULL, 1024, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
        head->size = 1024 - padded_block_size;
        head->next = NULL;
        head->in_use = 0;
    }

    struct block *b = head;

    // traverse the linked list
    while (b != NULL)
    {
        bool is_free = !b->in_use;
        bool has_enough_room = (b->size >= size);

        // split the current block if there's enough room
        if (is_free && has_enough_room)
        {
            int padded_requested_space = PADDED_SIZE(size);

            // space is big enough to split if current block size is greater than:
            // the requested size + the padded block + a minimum allocation of 16
            int required_space = (padded_requested_space + padded_block_size + ALIGNMENT);
            bool block_splittable = (bool)(b->size >= required_space);

            if (block_splittable)
            {
                // Add a new struct block with the remaining unused space
                struct block *new = NULL;
                new->size = (b->size - (padded_requested_space + padded_block_size));
                new->next = NULL;
                new->in_use = 0;

                // Wire it into the linked list
                b->next = new;
            }

            b->in_use = 1;
            b->size = padded_requested_space;

            return PTR_OFFSET(b, 0); // this offset is only true for the head, right?
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
    void *p = myalloc(17);
    print_data();
    printf("%p", p);

    return 0;
}
