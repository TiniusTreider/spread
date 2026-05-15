#include "csv.h"
#include "memory.h"
#include "stack.h"

#include <stddef.h>
#include <stdlib.h>

struct stack {
        size_t size;
        size_t occupancy;
        struct csv *data;
};

#define START_SIZE 8
#define GROW_SIZE 4

struct stack *s_init(void)
{
        struct stack *stack = smalloc(sizeof(struct stack));

        stack->size = START_SIZE;
        stack->occupancy = 0;
        stack->data = smalloc(stack->size * sizeof(struct csv));

        return stack;
}

void s_clean(struct stack *stack)
{
        for (size_t i = 0; i < stack->occupancy; i++)
        {
                csv_clean(stack->data[i]);
        }
        free(stack->data);
        free(stack);
}

struct csv s_index(struct stack *stack, size_t index)
{
        return stack->data[index];
}

static inline void s_grow(struct stack *stack)
{
        stack->size += GROW_SIZE;
        stack->data = srealloc(
                stack->data, stack->size * sizeof(struct csv)
        );
}

void s_push(struct stack *stack, struct csv c)
{
        stack->data[stack->occupancy] = c;
        stack->occupancy++;

        if (stack->occupancy == stack->size)
                s_grow(stack);
}

size_t s_size(struct stack *stack)
{
        return stack->occupancy;
}

