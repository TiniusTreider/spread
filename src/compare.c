#include "control.h"
#include "csv.h"
#include "stack.h"

#include <stddef.h>

static inline size_t range(struct csv a, struct csv b)
{
        /* TODO: find the range of days both sets have (bin search) */
}

static inline void compare_stocks(struct csv a, struct csv b)
{
        size_t length = range(a, b);
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

