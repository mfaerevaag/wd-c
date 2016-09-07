#include "persist_test.h"

/* setup */

static int setup(void **state)
{
    (void) state;

    /* path to config defined command line */
    set_rc_file(TEST_CONFIG);

    QUIET_FLAG = 1;

    wp_parse();

    return 0;
}

static int teardown(void **state)
{
    (void) state;

    wp_free();

    return 0;
}

/* tests */

static void test_find(void **state)
{
    (void) state;

    wp_print_all();

    wpoint *wp = wp_find("bar");

    assert_string_equal("bar", wp->name);
    assert_string_equal("/home/doge/foo/bar", wp->dir);
}

/* run */

int run_persist_tests()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_find),
    };

    return cmocka_run_group_tests_name(
        "persist_test",
        tests,
        setup,
        teardown);
}
