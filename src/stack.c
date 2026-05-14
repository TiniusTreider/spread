#include "memory.h"
#include "stack.h"

#include <stddef.h>
#include <stdlib.h>

struct stack {
        size_t size;
        size_t occupancy;
        struct date **data;
};

#define START_SIZE 8
#define GROW_SIZE 4

struct stack *s_init(void)
{
        struct stack *stack = smalloc(sizeof(struct stack));

        stack->size = START_SIZE;
        stack->occupancy = 0;
        stack->data = smalloc(stack->size * sizeof(struct date *));

        return stack;
}

void s_clean(struct stack *stack)
{
        free(stack->data);
        free(stack);
}

struct date *s_index(struct stack *stack, size_t index)
{
        return stack->data[index];
}

static inline void s_grow(struct stack *stack)
{
        stack->size += GROW_SIZE;
        stack->data = srealloc(
                stack->data, stack->size * sizeof(struct date*)
        );
}

void s_push(struct stack *stack, struct date *c)
{
        stack->data[stack->occupancy] = c;
        stack->occupancy++;

        if (stack->occupancy == stack->size)
                s_grow(stack);
}

