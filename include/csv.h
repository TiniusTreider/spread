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

struct date *csv_init(char*);
void csv_clean(struct date*);

#endif

