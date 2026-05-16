#include "control.h"
#include "csv.h"
#include "error.h"
#include "memory.h"
#include "stack.h"

#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void push_stock(struct stack *stack, char *name)
{
        size_t length = strlen(name);
        char *string = smalloc(length + sizeof(PATH));
        string[length] = '\0';
        memcpy(string, PATH, sizeof(PATH));
        strcat(string, name);

        printf("Reading \"%s\"...\n", string);

        const struct csv csv = csv_init(string);
        free(string);

        if (csv.dates != NULL) {
                s_push(stack, csv);
                printf("        %s pushed\n", csv.ticker);
        }
}

struct stack *stocks;

void parse(void)
{
        DIR *dir = opendir(PATH);
        errorif(dir == NULL, ERROR_OPEN_DIR);
        stocks = s_init();

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
                if (
                        strcmp(entry->d_name, ".") != 0 &&
                        strcmp(entry->d_name, "..") != 0 &&
                        strcmp(entry->d_name, "sc.py") != 0
                )
                        push_stock(stocks, entry->d_name);
        }

        errorif(closedir(dir) != 0, ERROR_CLOSE_DIR);
}

