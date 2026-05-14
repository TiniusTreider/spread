#include "control.h"
#include "csv.h"

#define PATH "/home/tinius/stocks/"

void parse(void)
{
        struct date *date = csv_init(PATH "amd.csv");
        csv_clean(date);
}

