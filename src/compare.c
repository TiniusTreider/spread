#include "control.h"
#include "csv.h"
#include "stack.h"

#include <stddef.h>

static inline void compare_stocks(struct csv a, struct csv b)
{
        (void)a;
        (void)b;
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

