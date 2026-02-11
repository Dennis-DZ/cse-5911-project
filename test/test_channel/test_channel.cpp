#include <unity.h>
#include "../test/MockHAL.h"
#include "../src/inputs/IInputSource.h"
#include "../src/outputs/PWMOutput.h"
#include "../src/control/Channel.h"

// Mock input source for testing
class MockInputSource : public IInputSource {
private:
    float value;
public:
    MockInputSource() : value(0.0) {}
    void setValue(float v) { value = v; }
    float getValue() override { return value; }
};

MockHAL hal;
MockInputSource* input;
PWMOutput* output;
Channel* channel;

void setUp(void) {
    input = new MockInputSource();
    output = new PWMOutput(&hal, 9);
    channel = new Channel(input, output);
}

void tearDown(void) {
    delete channel;
    delete output;
    delete input;
}

void test_channel_transfers_zero_value(void) {
    input->setValue(0.0);
    channel->update();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0, channel->getOutputValue());
    TEST_ASSERT_EQUAL(0, hal.getAnalogOutput(9));
}

void test_channel_transfers_max_value(void) {
    input->setValue(1.0);
    channel->update();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 1.0, channel->getOutputValue());
    TEST_ASSERT_EQUAL(255, hal.getAnalogOutput(9));
}

void test_channel_transfers_half_value(void) {
    input->setValue(0.5);
    channel->update();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.5, channel->getOutputValue());
    TEST_ASSERT_EQUAL(127, hal.getAnalogOutput(9));
}

void test_channel_updates_when_input_changes(void) {
    input->setValue(0.25);
    channel->update();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.25, channel->getOutputValue());
    
    input->setValue(0.75);
    channel->update();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.75, channel->getOutputValue());
}

void test_channel_maintains_independence(void) {
    // Set initial value
    input->setValue(0.6);
    channel->update();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.6, channel->getOutputValue());
    
    // Change input but don't update
    input->setValue(0.3);
    // Output should still be the old value
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.6, channel->getOutputValue());
    
    // After update, should have new value
    channel->update();
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.3, channel->getOutputValue());
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_channel_transfers_zero_value);
    RUN_TEST(test_channel_transfers_max_value);
    RUN_TEST(test_channel_transfers_half_value);
    RUN_TEST(test_channel_updates_when_input_changes);
    RUN_TEST(test_channel_maintains_independence);
    return UNITY_END();
}
