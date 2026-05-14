#ifndef CSV_H
#define CSV_H

#include <stddef.h>

struct date {
        size_t day;
        double close;
        size_t volume;
        double open;
        double high;
        double low;
};

struct date *csv_init(char*);
void csv_clean(struct date*);

#endif

