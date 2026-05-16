#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <stdbool.h>

#define ERROR_MAX_LENGTH 256

#define ERROR_OPEN_FILE "Failed to open file"
#define ERROR_NAVIGATE_FILE "Failed to navigate file"
#define ERROR_READ_FILE "Failed to read from file"
#define ERROR_CLOSE_FILE "Failed to close file"

#define ERROR_OPEN_DIR "Failed to open directory"
#define ERROR_CLOSE_DIR "Failed to close directory"

void error(const char*);
void errorif(bool, const char*);
void errorf(const char*, ...);
void erroriff(bool, const char*, ...);

#endif

