#ifndef CSV_H
#define CSV_H

#include <stddef.h>

struct date {
        size_t day;
        size_t close;
        size_t volume;
        size_t open;
        size_t high;
};

struct date *csv_init(char*);
void csv_clean(struct date*);

#endif

