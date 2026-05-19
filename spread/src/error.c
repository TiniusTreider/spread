#include "error.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void error(const char *message)
{
        fflush(stdout);
        fprintf(stderr, "error: %s\n", message);
        exit(EXIT_FAILURE);
}

void errorif(bool statement, const char *message)
{
        if (statement)
                error(message);
}

void errorf(const char *message, ...)
{
        va_list args;
        va_start(args, message);

        char buffer[ERROR_MAX_LENGTH];
        vsnprintf(buffer, ERROR_MAX_LENGTH, message, args);

        va_end(args);

        error(buffer);
}

void erroriff(bool statement, const char *message, ...)
{
        if (statement) {
                va_list args;
                va_start(args, message);

                char buffer[ERROR_MAX_LENGTH];
                vsnprintf(buffer, ERROR_MAX_LENGTH, message, args);

                va_end(args);

                error(buffer);
        }
}

