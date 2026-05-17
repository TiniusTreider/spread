#include "bar.h"
#include "control.h"
#include "csv.h"
#include "error.h"
#include "memory.h"
#include "stack.h"
#include "util.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static inline size_t choose_two(size_t x)
{
        return x * (x - 1) / 2;
}

struct range {
        size_t low;
        size_t high;
};

#define GOAL_MIDDLE " - "

static inline char *get_goal(struct csv a, struct csv b)
{
        size_t length =
                strlen(a.ticker) +
                strlen(b.ticker) +
                sizeof(GOAL_MIDDLE);

        char *string = smalloc(length);
        string[length - 1] = '\0';

        sprintf(string, "%s" GOAL_MIDDLE "%s", a.ticker, b.ticker);

        return string;
}


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

static inline struct date *get_spread(
        struct date *a, struct date *b, size_t length
) {
        struct date *spread = smalloc(length * sizeof(struct date));
        for (size_t i = 0; i < length; i++)
        {
                spread[i] = (struct date){
                        .day = a[i].day,
                        .close = logf(a[i].close / b[i].close)
                };
        }

        return spread;
}

struct linear {
        double m;
        double b;
};

static struct linear regression(struct date *spread, size_t length)
{
        double spread_sum = 0;
        for (size_t i = 0; i < length; i++)
        {
                spread_sum += spread[i].close;
        }
        const double mean_spread = spread_sum / length;

        double slope_sum = 0;
        double weight_sum = 0;
        for (size_t i = 0; i < length; i++)
        {
                const double weight = fabs((double)i - (double)length / 2);
                const double slope =
                        (spread[i].close - mean_spread) /
                        ((double)i - (double)length / 2);
                if (isinf(slope))
                        continue;
                slope_sum += slope * weight * weight;
                weight_sum += weight;
        }
        const double mean_slope = slope_sum / weight_sum;

        return (struct linear){
                .m = mean_slope,
                .b = mean_spread - mean_slope * (double)length / 2
        };
}

static inline double mean_square_error(
        struct date *spread, const struct linear lr, size_t length
) {
        double square_error_sum = 0;
        for (size_t i = 0; i < length; i++)
        {
                double error = (lr.m * i + lr.b) - spread[i].close;
                square_error_sum += error * error;
        }

        return square_error_sum / length;
}

#define LAG 10

static inline double mean_reversion(
        struct date *spread, const struct linear lr, size_t length
) {
        (void)spread;
        (void)lr;
        (void)length;

        double lag_sum = 0;
        double square_sum = 0;
        for (size_t i = 0; i < length - LAG; i++)
        {
                const double lag =
                        (i + LAG) * lr.m + lr.b - spread[i + LAG].close;
                const double epsilon = i * lr.m + lr.b - spread[i].close;

                lag_sum += lag * epsilon;
                square_sum += epsilon * epsilon;
        }

        return lag_sum / square_sum;
}

struct pair {
        char *a;
        char *b;
        double mse;
        double rho;
};

struct bar *bar;

static inline struct pair compare_stocks(struct csv a, struct csv b)
{
        printf_bar(bar, "%s - %s", a.ticker, b.ticker);

        struct range overlap = date_overlap(a, b);
        if (overlap.low > overlap.high)
                return (struct pair){};

        struct date *a_dates;
        struct date *b_dates;
        size_t length;
        if (overlap.low == a.dates[0].day)
        {
                a_dates = a.dates;
                b_dates = bin_search(b, overlap.low);
        } else {
                b_dates = b.dates;
                a_dates = bin_search(a, overlap.low);
        }
        struct date *a_end = bin_search(a, overlap.high);
        struct date *b_end = bin_search(b, overlap.high);
        length = MIN((a_end - a_dates + 1), (b_end - b_dates + 1));

        struct date *spread = get_spread(a_dates, b_dates, length);

        const struct linear lr = regression(spread, length);
        const double mse = mean_square_error(spread, lr, length);
        const double rho = mean_reversion(spread, lr, length);
        free(spread);
        printf_bar(bar, "        MSE: %lf\n        Rho: %lf", mse, rho);

        return (struct pair){
                .a = a.ticker, .b = b.ticker,
                .mse = mse, .rho = rho
        };
}

int compare_pairs(const void *a, const void *b)
{
        float x = 1 / ((struct pair*)a)->mse - ((struct pair*)a)->rho;
        float y = 1 / ((struct pair*)b)->mse - ((struct pair*)b)->rho;

        return (x > y) - (x < y);
}

#define WRITE_FILE "output.txt"

static inline void print_pairs(struct pair *pairs, size_t size)
{
        qsort(pairs, size, sizeof(struct pair), compare_pairs);

        for (size_t i = 0; i < size; i ++)
        {
                printf(
                        "        %2lu.     %-5s - %-5s    MSE: %lf, Rho: %lf\n",
                        size - i,
                        pairs[i].a, pairs[i].b,
                        pairs[i].mse, pairs[i].rho
                );

                if (size - i - 1 == 20)
                        printf("\n="
"=================================== TOP 20 ==================================="
                        "=\n\n");
                else if (size - i - 1 == 10)
                        printf("\n");
                else if (size - i - 1 == 3)
                        printf("\n");
        }

        FILE *file = fopen(WRITE_FILE, "w");
        errorif(file == NULL, ERROR_OPEN_FILE);

        fprintf(file, "A B MSE Rho\n");
        for (int i = size - 1; i >= 0; i--)
        {
                fprintf(
                        file, "%s %s %lf %lf\n",
                        pairs[i].a, pairs[i].b,
                        pairs[i].mse, pairs[i].rho
                );
        }

        errorif(fclose(file) != 0, ERROR_CLOSE_FILE);
}

void compare(void)
{
        size_t pair_count = choose_two(s_size(stocks));
        struct pair *pairs = smalloc(pair_count * sizeof(struct pair));

        bar = b_init(pair_count - 1);

        size_t i = 0;
        for (size_t a = 0; a < s_size(stocks); a++)
        {
                for (size_t b = a + 1; b < s_size(stocks); b++)
                {
                        struct csv csv_a = s_index(stocks, a);
                        struct csv csv_b = s_index(stocks, b);

                        pairs[i++] = compare_stocks(csv_a, csv_b);

                        char *string = get_goal(csv_a, csv_b);
                        bar_new_goal(bar, string);
                        free(string);
                }
        }

        b_clean(bar);
        print_pairs(pairs, pair_count);

        free(pairs);
        s_clean(stocks);
}

