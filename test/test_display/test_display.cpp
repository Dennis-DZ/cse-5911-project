#include <unity.h>
#include "../test/MockHAL.h"
#include "../src/display/DisplayManager.h"

// Enhanced MockHAL for display testing
class DisplayTestHAL : public MockHAL {
public:
    int initCallCount;
    int clearCallCount;
    int showCallCount;
    int setCursorCallCount;
    int printCallCount;
    
    DisplayTestHAL() : initCallCount(0), clearCallCount(0), 
                       showCallCount(0), setCursorCallCount(0),
                       printCallCount(0) {}
    
    void displayInit() override {
        initCallCount++;
    }
    
    void displayClear() override {
        clearCallCount++;
    }
    
    void displayShow() override {
        showCallCount++;
    }
    
    void displaySetCursor(int x, int y) override {
        setCursorCallCount++;
    }
    
    void displayPrint(const char* text) override {
        printCallCount++;
    }
};

DisplayTestHAL hal;
DisplayManager* display;

void setUp(void) {
    hal.initCallCount = 0;
    hal.clearCallCount = 0;
    hal.showCallCount = 0;
    hal.setCursorCallCount = 0;
    hal.printCallCount = 0;
    display = new DisplayManager(&hal);
}

void tearDown(void) {
    delete display;
}

void test_display_initializes(void) {
    display->initialize();
    TEST_ASSERT_GREATER_THAN(0, hal.initCallCount);
    TEST_ASSERT_GREATER_THAN(0, hal.clearCallCount);
    TEST_ASSERT_GREATER_THAN(0, hal.showCallCount);
}

void test_display_updates_with_values(void) {
    display->updateDisplay(0.5, 0.75);
    
    // Should clear, set cursor, print, and show
    TEST_ASSERT_GREATER_THAN(0, hal.clearCallCount);
    TEST_ASSERT_GREATER_THAN(0, hal.setCursorCallCount);
    TEST_ASSERT_GREATER_THAN(0, hal.printCallCount);
    TEST_ASSERT_GREATER_THAN(0, hal.showCallCount);
}

void test_display_clears_before_update(void) {
    int initialClearCount = hal.clearCallCount;
    display->updateDisplay(0.0, 1.0);
    TEST_ASSERT_GREATER_THAN(initialClearCount, hal.clearCallCount);
}

void test_display_shows_after_update(void) {
    int initialShowCount = hal.showCallCount;
    display->updateDisplay(0.25, 0.5);
    TEST_ASSERT_GREATER_THAN(initialShowCount, hal.showCallCount);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_display_initializes);
    RUN_TEST(test_display_updates_with_values);
    RUN_TEST(test_display_clears_before_update);
    RUN_TEST(test_display_shows_after_update);
    return UNITY_END();
}
