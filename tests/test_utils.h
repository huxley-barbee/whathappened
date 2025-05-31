#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stddef.h>

char **load_lines(const char *filename, size_t *count);
void free_lines(char **lines, size_t count);

#endif
