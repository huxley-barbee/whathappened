#include <setjmp.h>
#include <stddef.h>
#include <stdarg.h>
#include <cmocka.h>
#include "audit_line.h"
#include "test_utils.h"

static void test_parse_syscall_log(void **state) {
    size_t count;
    char **lines = load_lines("tests/data/syscall_execve_cwd.log", &count);
    assert_non_null(lines);
    assert_true(count > 0);

    for (size_t i = 0; i < count; ++i) {
        AuditRecord record = parse_audit_line(lines[i]);
        assert_true(record.valid);
    }

    free_lines(lines, count);

    /* need to use this var to get around errors.
     * not sure if cmocka need this in the signature.
     */
    (void)state;
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parse_syscall_log),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
