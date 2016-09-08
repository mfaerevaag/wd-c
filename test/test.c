#include "rc_test.h"
#include "engine_test.h"

int main()
{
    return run_rc_tests()
        || run_engine_tests();
}
