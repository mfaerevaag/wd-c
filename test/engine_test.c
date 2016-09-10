#include "engine_test.h"

/* setup */

static int setup(void **state)
{
    (void) state;

    /* path to config defined command line */
    rc_set_file(TEST_CONFIG);

    rc_parse();

    return 0;
}

static int teardown(void **state)
{
    (void) state;

    rc_free();

    return 0;
}

/* tests */

static void test_add(void **state)
{
    (void) state;

    assert_null(rc_find("asdf"));

    wd_add("asdf", "/a/s/d/f");

    wpoint *wp = rc_find("asdf");
    assert_non_null(wp);
    assert_string_equal("asdf", wp->name);
    assert_string_equal("/a/s/d/f", wp->dir);
}

static void test_remove(void **state)
{
    (void) state;

    assert_non_null(rc_find("bar"));

    wd_remove("bar");

    assert_null(rc_find("bar"));
}

/* run */

int run_engine_tests()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_add),
        cmocka_unit_test(test_remove),
    };

    return cmocka_run_group_tests_name(
        "engine_test",
        tests,
        setup,
        teardown);
}
