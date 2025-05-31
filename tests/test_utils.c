#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_utils.h"

char **load_lines(const char *filename, size_t *count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return NULL;
    }

    size_t cap = 128;
    *count = 0;
    char **lines = malloc(cap * sizeof(char *));
    char buffer[4096];

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (*count >= cap) {
            cap *= 2;
            lines = realloc(lines, cap * sizeof(char *));
        }
        lines[*count] = strdup(buffer);
        (*count)++;
    }

    fclose(fp);
    return lines;
}

void free_lines(char **lines, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        free(lines[i]);
    }
    free(lines);
}
