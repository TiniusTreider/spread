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

static inline struct date *bin_search(struct csv csv, size_t day)
{
        printf("        Searching...\n");

        struct date *low = csv.dates;
        struct date *high = csv.dates + csv.size - 1;

        while (high - low > 1)
        {
                struct date *middle = low + (high - low) / 2;
                if (middle->day > day) {
                        high = middle;
                } else {
                        low = middle;
                }
        }

        return low;
}

static inline void compare_stocks(struct csv a, struct csv b)
{
        printf("Comparing %s - %s ...\n", a.ticker, b.ticker);

        struct range overlap = date_overlap(a, b);
        if (overlap.low > overlap.high) {
                printf("        Stocks share no dates\n");
                return;
        }
        size_t length = overlap.high - overlap.low;
        printf("        Common history %lu - %lu\n", overlap.low, overlap.high);

        struct date *a_dates;
        struct date *b_dates;

        if (overlap.low == a.dates[0].day)
        {
                a_dates = a.dates;
                b_dates = bin_search(b, overlap.low);
        } else {
                b_dates = b.dates;
                a_dates = bin_search(a, overlap.low);
        }


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

