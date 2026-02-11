#include <unity.h>
#include "../test/MockHAL.h"
#include "../src/inputs/PotentiometerInput.h"

MockHAL hal;
PotentiometerInput* pot;

void setUp(void) {
    pot = new PotentiometerInput(&hal, 0);
}

void tearDown(void) {
    delete pot;
}

void test_potentiometer_reads_zero(void) {
    hal.setAnalogInput(0, 0);
    float value = pot->getValue();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, value);
}

void test_potentiometer_reads_max(void) {
    hal.setAnalogInput(0, 1023);
    float value = pot->getValue();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, value);
}

void test_potentiometer_reads_half(void) {
    hal.setAnalogInput(0, 512);
    float value = pot->getValue();
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.5, value);
}

void test_potentiometer_normalizes_correctly(void) {
    // Test quarter value
    hal.setAnalogInput(0, 256);
    float value = pot->getValue();
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.25, value);
    
    // Test three-quarter value
    hal.setAnalogInput(0, 768);
    value = pot->getValue();
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.75, value);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_potentiometer_reads_zero);
    RUN_TEST(test_potentiometer_reads_max);
    RUN_TEST(test_potentiometer_reads_half);
    RUN_TEST(test_potentiometer_normalizes_correctly);
    return UNITY_END();
}
