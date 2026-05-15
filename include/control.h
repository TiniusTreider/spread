#ifndef CONTROL_H
#define CONTROL_H

#include "stack.h"

#define PATH "scraper/"

extern struct stack *stocks;

void parse(void);
void compare(void);

#endif

