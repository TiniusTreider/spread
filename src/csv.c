#include "control.h"
#include "csv.h"
#include "error.h"
#include "io.h"
#include "memory.h"

#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static inline size_t line_length(char *line)
{
        char *c;
        c = strchr(line, '\n');
        if (c == NULL)
                c = line + strlen(line);

        return (size_t)(c - line);
}

const int month_days[] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};

static inline size_t unix_days(int day, int month, int year)
{
        errorif(
                day < 0 || day > 31 ||
                month < 1 || month > 12 ||
                year < 1970,
                "Invalid date"
        );

        const int u_year = year - 1970;
        int leap_years = month > 2 ? (u_year + 2) / 4 : (u_year + 1) / 4;

        return day + month_days[month - 1] + u_year * 365 + leap_years;
}

#define PARSE_FAIL "Failed to parse line"

static inline struct date parse_csv_line(char *file, size_t place)
{
        size_t length = line_length(file + place);

        char *string = smalloc(length + 1);
        string[length] = '\0';
        memcpy(string, file + place, length);

        struct date date = {};

        int month, day, year;

        errorif(
                sscanf(
                        string,
                        "%d/%d/%d,%f,%*s",
                        &month, &day, &year, &date.close
                ) != 4,
                PARSE_FAIL
        );
        free(string);

        date.day = unix_days(day, month, year);

        return date;
}

int compare_dates(const void *a, const void *b)
{
        const int x = (*(const struct date*)a).day;
        const int y = (*(const struct date*)b).day;

        return (x > y) - (x < y);
}

static inline struct date *parse_csv(
    char *file, size_t *lines, size_t line_count
) {
        struct date *dates = smalloc(line_count * sizeof(struct date));

        for (size_t i = 0; i < line_count - 1; i++)
        {
                dates[i] = parse_csv_line(file, lines[i]);
        }

        printf("        Sorting...\n");
        qsort(dates, line_count - 1, sizeof(struct date), compare_dates);

        return dates;
}

static inline void strupr(char *string)
{
        for (size_t i = 0; string[i] != '\0'; i++)
        {
                string[i] = toupper((string[i]));
        }
}

static inline char *ticker(char *path)
{
        size_t length = strlen(path) - sizeof(PATH) - 3;
        char *string = smalloc(length + 1);
        string[length] = '\0';
        memcpy(string, path + sizeof(PATH) - 1, length);

        strupr(string);

        return string;
}

struct csv csv_init(char *path)
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
        errorif(line_count == 0, "Empty stock");
        printf("        Dates: %lu\n", line_count);

        size_t *lines = smalloc(line_count * sizeof(size_t));
        size_t lines_fill = 0;
        size_t i = 0;
        while (file[i] != '\0')
        {
                if (file[i] == '\n')
                        lines[lines_fill++] = i + 1;

                i++;
        }

        struct date *dates = parse_csv(file, lines, line_count);

        free(lines);

        io_clean_file(file);

        return (struct csv){
                .dates = dates,
                .size = line_count - 1,
                .ticker = ticker(path)
        };
}

void csv_clean(struct csv csv)
{
        free(csv.dates);
        free(csv.ticker);
}

