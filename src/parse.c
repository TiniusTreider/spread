#include "control.h"
#include "csv.h"
#include "memory.h"
#include "stack.h"

#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH "/home/tinius/stocks/"

static inline void push_stock(struct stack *stack, char *name)
{
        size_t length = strlen(name);
        char *string = smalloc(length + sizeof(PATH));
        string[length] = '\0';
        memcpy(string, PATH, sizeof(PATH));
        strcat(string, name);

        printf("reading \"%s\"...\n", string);

        s_push(stack, csv_init(string));

        free(string);
}

void parse(void)
{
        DIR *dir = opendir(PATH);
        struct stack *stocks = s_init();

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
                if (
                        strcmp(entry->d_name, ".") != 0 &&
                        strcmp(entry->d_name, "..") != 0
                )
                        push_stock(stocks, entry->d_name);
        }
        closedir(dir);

        for (size_t i = 0; i < s_size(stocks); i++)
        {
                csv_clean(s_index(stocks, i));
        }
        s_clean(stocks);
}

