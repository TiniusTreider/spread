#include "bar.h"
#include "memory.h"
#include "util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bar {
        size_t max;
        size_t curr;
        char *goal;
};

#define GOAL_WIDTH 16
#define BAR_WIDTH 40
#define NUM_WIDTH 24

#define BAR_CHAR '='
#define BAR_ROOF '_'

#define STRING_H(A) #A
#define STRING(A) STRING_H(A)

struct bar *b_init(size_t max)
{
        struct bar *bar = smalloc(sizeof(struct bar));
        bar->max = max;
        bar->curr = 0;
        bar->goal = smalloc(1);
        bar->goal[0] = '\0';

        return bar;
}

void b_clean(struct bar *bar)
{
        free(bar->goal);
        free(bar);
}

void printf_bar(struct bar *bar, char *message, ...)
{
        va_list args;
        va_start(args, message);

        printf("\r%*s\033[F%*s\r",
                GOAL_WIDTH + BAR_WIDTH + NUM_WIDTH, "",
                GOAL_WIDTH + BAR_WIDTH + NUM_WIDTH, ""
        );
        vprintf(message, args);
        va_end(args);
        printf("\n");

        char roof[GOAL_WIDTH + BAR_WIDTH + NUM_WIDTH + 1];
        roof[GOAL_WIDTH + BAR_WIDTH + NUM_WIDTH] = '\0';
        memset(roof, BAR_ROOF, GOAL_WIDTH + BAR_WIDTH + NUM_WIDTH);
        printf("%s\n", roof);

        printf("|%-*s", GOAL_WIDTH - 1, bar->goal);

        size_t progress = MIN(
                (BAR_WIDTH - 2) * bar->curr / bar->max,
                BAR_WIDTH - 2
        );
        char string[BAR_WIDTH - 1];
        string[progress] = '\0';

        memset(string, BAR_CHAR, progress);
        printf("|%-*s|", BAR_WIDTH - 2, string);

        int precentage = MIN(100 * bar->curr / bar->max, 100);
        printf(" %d%% - %lu/%lu", precentage, bar->curr, bar->max);
        fflush(stdout);
}

void bar_new_goal(struct bar *bar, char *goal)
{
        bar->curr++;

        free(bar->goal);

        size_t length = strlen(goal);
        bar->goal = smalloc(length + 1);
        bar->goal[length] = '\0';
        memcpy(bar->goal, goal, length);
}

