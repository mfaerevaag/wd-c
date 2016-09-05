#include "config_test.h"
#include "persist_test.h"

int main()
{
    return run_config_tests()
        || run_persist_tests();
}
