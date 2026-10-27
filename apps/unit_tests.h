#include <stdio.h>

#define TEST(func, expected, msg) { if ((func) != (expected)) { printf("%s Test Fail ❌\n", msg); } else { printf("%s Test Pass ✅\n", msg); } }

void test_heap_init();
