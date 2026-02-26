#include <stdlib.h>
#include <stdio.h>

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "Error: %s\nFile: %s\nLine: %d\n", \
                    #expr, __FILE__, __LINE__); \
            perror("System error"); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)