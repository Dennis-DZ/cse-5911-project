#include <unity.h>
#include "../test/MockHAL.h"
#include "../src/MedicalDeviceController.h"

MockHAL hal;
MedicalDeviceController* controller;

void setUp(void) {
    controller = new MedicalDeviceController(&hal);
}

void tearDown(void) {
    delete controller;
}

void test_controller_initializes(void) {
    // Should not crash
    controller->initialize();
    TEST_ASSERT_TRUE(true);
}

void test_controller_reads_pots_and_sets_outputs(void) {
    // Set potentiometer values
    hal.setAnalogInput(0, 512);  // 50% on pot 1
    hal.setAnalogInput(1, 256);  // 25% on pot 2
    
    controller->initialize();
    controller->update();
    
    // Check outputs are set correctly
    // 50% should map to PWM value of 127
    TEST_ASSERT_EQUAL(127, hal.getAnalogOutput(9));
    // 25% should map to PWM value of 63
    TEST_ASSERT_EQUAL(63, hal.getAnalogOutput(10));
}

void test_controller_updates_continuously(void) {
    controller->initialize();
    
    // First update
    hal.setAnalogInput(0, 0);
    hal.setAnalogInput(1, 0);
    controller->update();
    TEST_ASSERT_EQUAL(0, hal.getAnalogOutput(9));
    TEST_ASSERT_EQUAL(0, hal.getAnalogOutput(10));
    
    // Second update with different values
    hal.setAnalogInput(0, 1023);
    hal.setAnalogInput(1, 512);
    controller->update();
    TEST_ASSERT_EQUAL(255, hal.getAnalogOutput(9));
    TEST_ASSERT_EQUAL(127, hal.getAnalogOutput(10));
}

void test_controller_channels_are_independent(void) {
    controller->initialize();
    
    // Set channel 1 high, channel 2 low
    hal.setAnalogInput(0, 1023);
    hal.setAnalogInput(1, 0);
    controller->update();
    TEST_ASSERT_EQUAL(255, hal.getAnalogOutput(9));
    TEST_ASSERT_EQUAL(0, hal.getAnalogOutput(10));
    
    // Reverse them
    hal.setAnalogInput(0, 0);
    hal.setAnalogInput(1, 1023);
    controller->update();
    TEST_ASSERT_EQUAL(0, hal.getAnalogOutput(9));
    TEST_ASSERT_EQUAL(255, hal.getAnalogOutput(10));
}

void test_controller_handles_edge_cases(void) {
    controller->initialize();
    
    // Test with exact midpoint values
    hal.setAnalogInput(0, 511);
    hal.setAnalogInput(1, 512);
    controller->update();
    
    // Should handle rounding appropriately
    int output1 = hal.getAnalogOutput(9);
    int output2 = hal.getAnalogOutput(10);
    TEST_ASSERT_TRUE(output1 >= 126 && output1 <= 128);
    TEST_ASSERT_TRUE(output2 >= 126 && output2 <= 128);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_controller_initializes);
    RUN_TEST(test_controller_reads_pots_and_sets_outputs);
    RUN_TEST(test_controller_updates_continuously);
    RUN_TEST(test_controller_channels_are_independent);
    RUN_TEST(test_controller_handles_edge_cases);
    return UNITY_END();
}
