#ifndef CSV_H
#define CSV_H

#include <stddef.h>

struct date {
        size_t day;
        float close;
        size_t volume;
        float open;
        float high;
        float low;
};

struct csv {
        struct date *dates;
        size_t size;
};

struct csv csv_init(char*);
void csv_clean(struct csv);

#endif

