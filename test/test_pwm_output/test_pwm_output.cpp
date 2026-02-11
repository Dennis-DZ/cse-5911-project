#include <unity.h>
#include "../test/MockHAL.h"
#include "../src/outputs/PWMOutput.h"

MockHAL hal;
PWMOutput* output;

void setUp(void) {
    output = new PWMOutput(&hal, 9);
}

void tearDown(void) {
    delete output;
}

void test_pwm_output_sets_zero(void) {
    output->setValue(0.0);
    TEST_ASSERT_EQUAL(0, hal.getAnalogOutput(9));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, output->getValue());
}

void test_pwm_output_sets_max(void) {
    output->setValue(1.0);
    TEST_ASSERT_EQUAL(255, hal.getAnalogOutput(9));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, output->getValue());
}

void test_pwm_output_sets_half(void) {
    output->setValue(0.5);
    TEST_ASSERT_EQUAL(127, hal.getAnalogOutput(9));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, output->getValue());
}

void test_pwm_output_clamps_high_value(void) {
    output->setValue(1.5);
    TEST_ASSERT_EQUAL(255, hal.getAnalogOutput(9));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, output->getValue());
}

void test_pwm_output_clamps_negative_value(void) {
    output->setValue(-0.5);
    TEST_ASSERT_EQUAL(0, hal.getAnalogOutput(9));
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, output->getValue());
}

void test_pwm_output_converts_correctly(void) {
    output->setValue(0.25);
    TEST_ASSERT_EQUAL(63, hal.getAnalogOutput(9));
    
    output->setValue(0.75);
    TEST_ASSERT_EQUAL(191, hal.getAnalogOutput(9));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_pwm_output_sets_zero);
    RUN_TEST(test_pwm_output_sets_max);
    RUN_TEST(test_pwm_output_sets_half);
    RUN_TEST(test_pwm_output_clamps_high_value);
    RUN_TEST(test_pwm_output_clamps_negative_value);
    RUN_TEST(test_pwm_output_converts_correctly);
    return UNITY_END();
}
