#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "audit_line.h"

static void safe_copy(char *dest, const char *src, size_t max_len) {
    size_t copy_len = strnlen(src, max_len - 1);
    memcpy(dest, src, copy_len);
    dest[copy_len] = '\0';
}

AuditRecord parse_audit_line(const char *line) {
    AuditRecord rec;
    rec.valid = 0;
    rec.field_count = 0;
    rec.type[0] = '\0';
    rec.msg_prefix[0] = '\0';

    if (strncmp(line, "type=", 5) != 0) {
        return rec;
    }

    const char *type_start = line + 5;
    const char *space_after_type = strchr(type_start, ' ');
    if (!space_after_type) {
        return rec;
    }

    size_t type_len = space_after_type - type_start;
    if (type_len >= MAX_TYPE_LEN) {
        type_len = MAX_TYPE_LEN - 1;
    }
    safe_copy(rec.type, type_start, type_len + 1);

    const char *msg_start = strstr(space_after_type, "msg=");
    if (!msg_start) {
        return rec;
    }
    msg_start += 4;

    const char *kv_start = strchr(msg_start, ':');
    if (!kv_start) {
        return rec;
    }
    kv_start++; // move past ':'

    // Extract msg_prefix (the audit(....): part)
    size_t prefix_len = kv_start - msg_start;
    if (prefix_len >= MAX_MSG_PREFIX_LEN) {
        prefix_len = MAX_MSG_PREFIX_LEN - 1;
    }
    safe_copy(rec.msg_prefix, msg_start, prefix_len + 1);

    // Parse key=value pairs
    const char *p = kv_start;
    while (*p && rec.field_count < MAX_FIELDS) {
        while (*p == ' ') p++;  // skip spaces

        const char *eq = strchr(p, '=');
        if (!eq) break;

        size_t key_len = eq - p;
        if (key_len >= MAX_KEY_LEN) key_len = MAX_KEY_LEN - 1;
        safe_copy(rec.fields[rec.field_count].key, p, key_len + 1);

        const char *val_start = eq + 1;
        const char *val_end = strchr(val_start, ' ');
        if (!val_end) val_end = val_start + strlen(val_start);

        size_t val_len = val_end - val_start;
        if (val_len >= MAX_VALUE_LEN) val_len = MAX_VALUE_LEN - 1;
        safe_copy(rec.fields[rec.field_count].value, val_start, val_len + 1);

        rec.field_count++;
        p = val_end;
    }

    rec.valid = 1;
    return rec;
}

