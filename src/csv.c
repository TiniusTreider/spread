#include "csv.h"
#include "io.h"

#include <stdlib.h>

struct date *csv_init(char *path)
{
        (void)path;
}

void *csv_clean(struct date *dates)
{
        free(dates);
}

