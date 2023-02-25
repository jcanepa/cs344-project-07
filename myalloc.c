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

        if (is_free && has_enough_room)
        {
            int padded_requested_space = PADDED_SIZE(size);

            // space is big enough to split if current block size is greater than:
            // the requested size + the padded block + a minimum allocation of 16
            int required_space = (padded_requested_space + padded_block_size + ALIGNMENT);
            bool block_splittable = (bool)(b->size >= required_space);

            if (block_splittable)
            {
                // create new block for the remaining unused space
                int new_size = (b->size - (padded_requested_space + padded_block_size));
                struct block new = {
                    .next = NULL,
                    .size = new_size,
                    .in_use = 0};

                // printf("Required space: %d\n", required_space);
                // printf("Padded requested space: %d\n", padded_requested_space);
                // printf("Padded block size: %d\n", padded_block_size);
                printf(
                    "New node: [%d,%s]\n",
                    new_size,
                    (new.in_use ? "used" : "free"));

                // wire it into the linked list
                b->next = &new;

                // memcpy(
                //     b + (padded_requested_space + padded_block_size),
                //     &new,
                //     sizeof(struct block));
            }

            b->in_use = 1;
            b->size = padded_requested_space;

            return PTR_OFFSET(b, padded_block_size);
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
    if (p == NULL)
        return;

    // navigate back to header block
    struct block *b = (p - sizeof(struct block));
    b->in_use = 0;
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
    // ====== Example 1 ======
    void *p;

    p = myalloc(512);
    print_data();

    myfree(p);
    print_data();

    printf("EXPECTED\n");
    printf("[512,used] -> [480,free]\n");
    printf("[512,free] -> [480,free]\n\n\n");

    // ====== Example 2 ======
    head = NULL;

    myalloc(10);
    print_data();
    myalloc(20);
    print_data();
    myalloc(30);
    print_data();
    myalloc(40);
    print_data();
    myalloc(50);
    print_data();

    // ====== Example 3 ======
    head = NULL;

    myalloc(10);
    print_data();
    p = myalloc(20);
    print_data();
    myalloc(30);
    print_data();
    myfree(p);
    print_data();
    myalloc(40);
    print_data();
    myalloc(10);
    print_data();
}
