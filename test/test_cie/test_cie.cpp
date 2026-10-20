#include <unity.h>

#include "core/Cie1931.h"

void test_endpoints() {
    TEST_ASSERT_EQUAL_UINT16(0, cie1931(0));
    TEST_ASSERT_EQUAL_UINT16(1023, cie1931(100));
    TEST_ASSERT_EQUAL_UINT16(1023, cie1931(255));
}

void test_known_values() {
    TEST_ASSERT_EQUAL_UINT16(1, cie1931(1));
    TEST_ASSERT_EQUAL_UINT16(9, cie1931(8));
    TEST_ASSERT_EQUAL_UINT16(10, cie1931(9));
    TEST_ASSERT_EQUAL_UINT16(188, cie1931(50));
    TEST_ASSERT_EQUAL_UINT16(996, cie1931(99));
}

void test_monotonic() {
    uint16_t previous = cie1931(0);
    for (uint8_t percent = 1; percent <= 100; ++percent) {
        const uint16_t current = cie1931(percent);
        TEST_ASSERT_TRUE(current >= previous);
        previous = current;
    }
}

int main(int, char **) {
    UNITY_BEGIN();
    RUN_TEST(test_endpoints);
    RUN_TEST(test_known_values);
    RUN_TEST(test_monotonic);
    return UNITY_END();
}
