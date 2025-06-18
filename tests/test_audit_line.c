#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include "audit_line.h"
#include "test_utils.h"

static char **lines = NULL;
static size_t count = 0;

static int load_fixture(void **state) {
    /* need to use this var to get around errors.
     * not sure if cmocka need this in the signature.
     */
    (void)state;
    lines = load_lines("tests/data/syscall_execve_cwd.log", &count);
    if (!lines) {
        return -1;
    }
    return 0;
}

static int free_fixture(void **state) {
    /* need to use this var to get around errors.
     * not sure if cmocka need this in the signature.
     */
    (void)state;
    free_lines(lines, count);
    lines = NULL;
    count = 0;
    return 0;
}

static void test_parse_syscall_log(void **state) {
    /* need to use this var to get around errors.
     * not sure if cmocka need this in the signature.
     */
    (void)state;
    assert_non_null(lines);
    assert_true(count > 0);

    for (size_t i = 0; i < count; ++i) {
        AuditRecord record = parse_audit_line(lines[i]);
        assert_true(record.valid);
        assert_true(strlen(record.type) > 0);
        assert_non_null(strstr(record.msg_prefix, "audit("));
        assert_true(record.field_count > 0);

        for (size_t j = 0; j < record.field_count; j++) {
            assert_true(strlen(record.fields[j].key) > 0);
            assert_true(strlen(record.fields[j].value) > 0);
        }
    }
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_parse_syscall_log, load_fixture, free_fixture),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

