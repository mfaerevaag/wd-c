#include "persist_test.h"

/* setup */

static int setup(void **state)
{
    (void) state;

    /* meta_init(); */

    return 0;
}

static int teardown(void **state)
{
    (void) state;

    /* meta_free(); */

    return 0;
}

/* tests */

static void test_foobar(void **state)
{
    (void) state;

    assert_int_equal(8, 8);
}

/* run */

int run_persist_tests()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_foobar),
    };

    return cmocka_run_group_tests_name(
        "persist_test",
        tests,
        setup,
        teardown);
}
