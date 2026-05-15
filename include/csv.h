#ifndef CSV_H
#define CSV_H

#include <stddef.h>

struct date {
        size_t day;
        float close;
};

struct csv {
        struct date *dates;
        size_t size;
        char *ticker;
};

struct csv csv_init(char*);
void csv_clean(struct csv);

#endif

