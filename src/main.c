#include "control.h"
#include "error.h"

#include <string.h>

#define PARSE "parse"
#define COMPARE "comp"

#define ARGUMENT_COUNT "Needs 1 argument (\"" PARSE "\" or \"" COMPARE "\")"
#define ARGUMENTS "Argument not \"" PARSE "\" or \"" COMPARE "\""

int main(int argc, char **argv)
{
        errorif(argc != 2, ARGUMENT_COUNT);
        char *arg = argv[1];

        if (strcmp(arg, PARSE) == 0)
                parse();
        else if (strcmp(arg, COMPARE) == 0)
                compare();
        else
                error(ARGUMENTS);

        return 0;
}

