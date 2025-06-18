#pragma once

#include <stddef.h>

#define MAX_TYPE_LEN 16
#define MAX_MSG_PREFIX_LEN 256
#define MAX_FIELDS 32
#define MAX_KEY_LEN 64
#define MAX_VALUE_LEN 256

typedef struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
} AuditKV;

typedef struct {
    int valid;
    char type[MAX_TYPE_LEN];
    char msg_prefix[MAX_MSG_PREFIX_LEN];
    AuditKV fields[MAX_FIELDS];
    size_t field_count;
} AuditRecord;

AuditRecord parse_audit_line(const char *line);

