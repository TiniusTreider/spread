#ifndef STACK_H
#define STACK_H

#include <stddef.h>

struct stack;

struct stack *s_init(void);
void s_clean(struct stack*);
struct date *s_index(struct stack*, size_t);
void s_push(struct stack*, struct date*);
size_t s_size(struct stack*);

#endif

