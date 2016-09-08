#include "rc_test.h"

/* setup */

static int setup(void **state)
{
    (void) state;

    return 0;
}

static int teardown(void **state)
{
    (void) state;

    rc_free();

    return 0;
}

/* tests */

static void test_rc_get(void **state)
{
    (void) state;
    char *home = getenv("HOME");
    char *rc = ".warprc";

    char *expected = malloc(strlen(home) + strlen(rc) + 1);
    sprintf(expected, "%s/%s", home, rc);

    char *actual = rc_get_file();

    assert_string_equal(expected, actual);
}

static void test_rc_set(void **state)
{
    (void) state;


    char *expected = TEST_CONFIG;

    rc_set_file(expected);

    char *actual = rc_get_file();

    assert_string_equal(expected, actual);
}

static void test_rc_find_index(void **state)
{
    (void) state;

    /* path to config defined command line */
    rc_set_file(TEST_CONFIG);

    int index = rc_find_index("bar");

    assert_int_equal(1, index);
    assert_string_equal("bar", rc_tab()->points[index]->name);
    assert_string_equal("/home/doge/foo/bar", rc_tab()->points[index]->dir);
}

static void test_rc_find(void **state)
{
    (void) state;

    /* path to config defined command line */
    rc_set_file(TEST_CONFIG);

    wpoint *wp = rc_find("bar");

    assert_non_null(wp);
    assert_string_equal("bar", wp->name);
    assert_string_equal("/home/doge/foo/bar", wp->dir);
}

/* run */

int run_rc_tests()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_rc_get),
        cmocka_unit_test(test_rc_set),
        cmocka_unit_test(test_rc_find_index),
        cmocka_unit_test(test_rc_find),
    };

    return cmocka_run_group_tests_name(
        "rc_test",
        tests,
        setup,
        teardown);
}
