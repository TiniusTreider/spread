#include "control.h"
#include "csv.h"
#include "stack.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

struct range {
        size_t low;
        size_t high;
};

static inline struct range range(struct csv a, struct csv b)
{
        (void)a;
        (void)b;
        return (struct range){ .low = 1, .high = 0 };
}

static inline void compare_stocks(struct csv a, struct csv b)
{
        printf("Comparing %s - %s ...\n", a.ticker, b.ticker);

        struct range overlap = range(a, b);
        if (overlap.low > overlap.high) {
                printf("Stocks share no dates\n");
                return;
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

