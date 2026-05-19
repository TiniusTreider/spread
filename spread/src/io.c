#include "error.h"
#include "io.h"
#include "memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

char *io_read_file(char *restrict path)
{
        FILE *file = fopen(path, "r");
        errorif(file == NULL, ERROR_OPEN_FILE);

        errorif(fseek(file, 0, SEEK_END) != 0, ERROR_NAVIGATE_FILE);
        const long size = ftell(file);
        errorif(size == -1, ERROR_NAVIGATE_FILE);

        char *buffer = smalloc(size + 1);
        buffer[size] = '\0';

        errorif(fseek(file, 0, SEEK_SET) != 0, ERROR_NAVIGATE_FILE);

        const size_t read = fread(buffer, 1, size, file);
        errorif(read != (size_t)size, ERROR_READ_FILE);

        errorif(fclose(file) != 0, ERROR_CLOSE_FILE);

        return buffer;
}

void io_clean_file(char *buffer)
{
        free(buffer);
}

