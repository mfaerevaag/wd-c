#include "config_test.h"

/* setup */

static int setup(void **state)
{
    (void) state;

    return 0;
}

static int teardown(void **state)
{
    (void) state;

    return 0;
}

/* tests */

static void test_get_rc(void **state)
{
    (void) state;
    char *home = getenv("HOME");
    char *rc = ".warprc";

    char *expected = malloc(strlen(home) + strlen(rc) + 1);
    sprintf(expected, "%s/%s", home, rc);

    char *actual = get_rc_file();

    assert_string_equal(expected, actual);
}

static void test_set_rc(void **state)
{
    (void) state;

    char *expected = "/some/where/else/warprc";

    set_rc_file(expected);

    char *actual = get_rc_file();

    assert_string_equal(expected, actual);
}

/* run */

int run_config_tests()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_rc),
        cmocka_unit_test(test_set_rc),
    };

    return cmocka_run_group_tests_name(
        "config_test",
        tests,
        setup,
        teardown);
}
