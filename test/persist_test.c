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

static void test_find_id(void **state)
{
    (void) state;

    int index = wp_find_index("bar");

    assert_int_equal(1, index);
    assert_string_equal("bar", wp_all()[index]->name);
    assert_string_equal("/home/doge/foo/bar", wp_all()[index]->dir);
}

static void test_find(void **state)
{
    (void) state;

    wpoint *wp = wp_find("bar");

    assert_non_null(wp);
    assert_string_equal("bar", wp->name);
    assert_string_equal("/home/doge/foo/bar", wp->dir);
}

static void test_add(void **state)
{
    (void) state;

    assert_null(wp_find("asdf"));

    wp_add("asdf", "/a/s/d/f");

    wpoint *wp = wp_find("asdf");
    assert_non_null(wp);
    assert_string_equal("asdf", wp->name);
    assert_string_equal("/a/s/d/f", wp->dir);
}

static void test_remove(void **state)
{
    (void) state;

    assert_non_null(wp_find("bar"));

    wp_remove("bar");

    assert_null(wp_find("bar"));
}

/* run */

int run_persist_tests()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_find),
        cmocka_unit_test(test_find_id),
        cmocka_unit_test(test_add),
        cmocka_unit_test(test_remove),
    };

    return cmocka_run_group_tests_name(
        "persist_test",
        tests,
        setup,
        teardown);
}
