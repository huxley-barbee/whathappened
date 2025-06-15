#include "audit_line.h"
#include <stdio.h>
#include <string.h>

AuditRecord parse_audit_line(const char *line) {
    AuditRecord rec = {.valid = 0};

    if (sscanf(line, "type=%15[^ ] msg=%4095[^ ]", rec.type,
	rec.msg) == 2) {

        rec.valid = 1;
    }

    return rec;
}
