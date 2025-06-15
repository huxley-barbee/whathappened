#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_utils.h"

char *safe_strndup(const char *s, size_t maxlen) {
    size_t len = strnlen(s, maxlen);
    char *copy = malloc(len + 1);
    if (!copy) return NULL;
    memcpy(copy, s, len);
    copy[len] = '\0';
    return copy;
}

char **load_lines(const char *filename, size_t *count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return NULL;
    }

    size_t cap = 128;
    *count = 0;

    char **lines = calloc(cap, sizeof(char *));
    if (!lines) {
        perror("calloc");
        fclose(fp);
        return NULL;
    }

    fprintf(stderr, "[ALLOC] lines allocated at %p (cap=%zu)\n", (void*)lines, cap);

    char buffer[4096];

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (*count >= cap) {
            size_t new_cap = cap * 2;
            char **tmp = malloc(new_cap * sizeof(char *));
            if (!tmp) {
                perror("malloc");
                free_lines(lines, *count);
                fclose(fp);
                return NULL;
            }
            memcpy(tmp, lines, cap * sizeof(char *));
            free(lines);
            lines = tmp;
            cap = new_cap;
            fprintf(stderr, "[REALLOC] lines moved to %p (new cap=%zu)\n", (void*)lines, cap);
        }

        lines[*count] = safe_strndup(buffer, sizeof(buffer));
        if (!lines[*count]) {
            perror("safe_strndup");
            free_lines(lines, *count);
            fclose(fp);
            return NULL;
        }

        (*count)++;
    }

    fclose(fp);
    return lines;
}

void free_lines(char **lines, size_t count) {
    if (!lines) {
        fprintf(stderr, "[FREE] called with NULL\n");
        return;
    }

    fprintf(stderr, "[FREE] freeing lines at %p (count=%zu)\n", (void*)lines, count);

    for (size_t i = 0; i < count; ++i) {
        free(lines[i]);
    }
    free(lines);
}

