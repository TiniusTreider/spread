#include "bar.h"
#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bar {
        size_t max;
        size_t curr;
        char *goal;
};

#define BAR_WIDTH 48
#define GOAL_WIDTH 16
#define NUM_WIDTH 16

void print_bar(struct bar *bar, char *message)
{
        size_t fill = BAR_WIDTH * (bar->curr / bar->max);
        int percentage = 100 * (bar->curr / bar->max);

        char *string = smalloc(BAR_WIDTH + 1);
        string[BAR_WIDTH] = '\0';

        memset(string, ' ', GOAL_WIDTH);
        snprintf(string, GOAL_WIDTH, "%s", bar->goal);
        memset(string + GOAL_WIDTH, '#', fill);
        memset(string + GOAL_WIDTH + fill, ' ', BAR_WIDTH - fill + NUM_WIDTH);
        snprintf(
                string + GOAL_WIDTH + BAR_WIDTH, NUM_WIDTH,
                "%d%% - %lu/%lu",
                percentage,
                bar->curr, bar->max
        );

        printf("\r%s\n%s", message, string);
        free(string);
}

void new_goal(struct bar *bar, char *goal)
{
        bar->curr++;
        bar->goal = goal;
}

