#include "csv.h"
#include "io.h"
#include "memory.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static inline size_t line_length(char *line)
{
        char *c = strchr(line, '\n');
        if (c != NULL)
                return (size_t)(c - line);

        return strlen(line);
}

static inline struct date parse_csv_line(char *file, size_t place)
{
        size_t length = line_length(file + place);

        char *string = smalloc(length + 1);
        string[length] = '\0';
        memcpy(string, file + place, length);

        struct date date = {};

        printf("%s\n", string);

        return date;
}

static inline struct date *parse_csv(
    char *file, size_t *lines, size_t line_count
) {
        struct date *dates = smalloc(line_count * sizeof(struct date));

        for (size_t i = 0; i < line_count; i++)
        {
                dates[i] = parse_csv_line(file, lines[i]);
        }

        return dates;
}

struct date *csv_init(char *path)
{
        char *file = io_read_file(path);

        size_t line_count = 0;
        size_t length = 0;
        while (file[length] != '\0')
        {
                if (file[length] == '\n')
                        line_count++;

                length++;
        }
        printf(
            "path: %s\n"
            "dates: %lu\n",
            path, line_count
        );

        size_t *lines = smalloc(line_count * sizeof(size_t));
        size_t lines_fill = 0;
        size_t i = 0;
        while (file[i] != '\0')
        {
                if (file[i] == '\n')
                        lines[lines_fill++] = i;

                i++;
        }

        struct date *dates = parse_csv(file, lines, line_count);

        io_clean_file(file);

        return dates;
}

void csv_clean(struct date *dates)
{
        free(dates);
}

