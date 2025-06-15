#pragma once

typedef struct {
    char type[16];
    char msg[4096];
    int valid;
} AuditRecord;

AuditRecord parse_audit_line(const char *line);
