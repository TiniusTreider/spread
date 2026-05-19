#ifndef BAR_H
#define BAR_H

#include <stddef.h>

struct bar;

struct bar *b_init(size_t);
void b_clean(struct bar*);
void printf_bar(struct bar*, char*, ...);
void bar_new_goal(struct bar*, char*);

#endif

