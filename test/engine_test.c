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

    int rc = wd_add("asdf", "/a/s/d/f");
    assert_int_equal(0, rc);

    wpoint *wp = rc_find("asdf");
    assert_non_null(wp);
    assert_string_equal("asdf", wp->name);
    assert_string_equal("/a/s/d/f", wp->dir);

    wd_remove("asdf");
}

static void test_add_space_name(void **state)
{
    (void) state;

    assert_null(rc_find("a s d f"));

    int rc = wd_add("a s d f", "/a/s/d/f");
    assert_int_equal(0, rc);

    wpoint *wp = rc_find("a s d f");
    assert_non_null(wp);
    assert_string_equal("a s d f", wp->name);
    assert_string_equal("/a/s/d/f", wp->dir);

    wd_remove("a s d f");
}

static void test_add_space_dir(void **state)
{
    (void) state;

    assert_null(rc_find("a s d f"));

    int rc = wd_add("a s d f", "/a s d f/s/d/f U");
    assert_int_equal(0, rc);

    wpoint *wp = rc_find("a s d f");
    assert_non_null(wp);
    assert_string_equal("a s d f", wp->name);
    assert_string_equal("/a s d f/s/d/f U", wp->dir);

    wd_remove("a s d f");
}

static void test_add_colon_name(void **state)
{
    (void) state;

    int rc = wd_add("as:df", "/a/s/d/f");

    assert_int_equal(1, rc);
}

static void test_add_colon_dir(void **state)
{
    (void) state;

    int rc = wd_add("asdf", "/a/s:/d/f");

    assert_int_equal(1, rc);
}

static void test_remove(void **state)
{
    (void) state;

    assert_null(rc_find("asdf"));

    wd_add("asdf", "/a/s/d/f");

    assert_non_null(rc_find("asdf"));

    int rc = wd_remove("asdf");
    assert_int_equal(0, rc);

    assert_null(rc_find("asdf"));
}

static void test_show(void **state)
{
    (void) state;

    char *pwd = getenv("PWD");

    wd_add("asdfasdf", pwd);

    assert_string_equal("asdfasdf", wd_show(pwd));

    wd_remove("asdfasdf");
}

static void test_path(void **state)
{
    (void) state;

    wd_add("asdfasdf", "/a/s/d/f");

    assert_string_equal("/a/s/d/f", wd_path("asdfasdf"));

    wd_remove("asdfasdf");
}

/* run */

int run_engine_tests()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_add),
        cmocka_unit_test(test_add_space_name),
        cmocka_unit_test(test_add_space_dir),
        cmocka_unit_test(test_add_colon_name),
        cmocka_unit_test(test_remove),
        cmocka_unit_test(test_show),
        cmocka_unit_test(test_path),
    };

    return cmocka_run_group_tests_name(
        "engine_test",
        tests,
        setup,
        teardown);
}
