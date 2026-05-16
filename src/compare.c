#include "control.h"
#include "csv.h"
#include "stack.h"
#include "util.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

struct range {
        size_t low;
        size_t high;
};

static inline struct range date_overlap(struct csv a, struct csv b)
{
        struct range ra = (struct range){
                .low = a.dates[0].day,
                .high = a.dates[a.size - 1].day
        };
        struct range rb = (struct range){
                .low = b.dates[0].day,
                .high = b.dates[b.size - 1].day
        };
        return (struct range){
                .low = MAX(ra.low, rb.low),
                .high = MIN(ra.high, rb.high)
        };
}

static inline void compare_stocks(struct csv a, struct csv b)
{
        printf("Comparing %s - %s ...\n", a.ticker, b.ticker);

        struct range overlap = date_overlap(a, b);
        if (overlap.low > overlap.high) {
                printf("        Stocks share no dates\n");
                return;
        }
        printf("        Common history %lu - %lu\n", overlap.low, overlap.high);
}

void compare(void)
{
        for (size_t a = 0; a < s_size(stocks); a++)
        {
                for (size_t b = a + 1; b < s_size(stocks); b++)
                {
                        compare_stocks(s_index(stocks, a), s_index(stocks, b));
                }
        }

        s_clean(stocks);
}

