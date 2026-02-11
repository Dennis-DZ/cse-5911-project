#ifndef MOCK_HAL_H
#define MOCK_HAL_H

#include "../src/hal/IHAL.h"
#include <map>

/**
 * Mock Hardware Abstraction Layer for Testing
 * 
 * Simulates hardware behavior for unit tests without requiring
 * actual hardware. Stores pin values in memory.
 */
class MockHAL : public IHAL {
private:
    std::map<int, int> analogInputs;
    std::map<int, int> analogOutputs;
    unsigned long mockMillis;
    
public:
    MockHAL() : mockMillis(0) {}
    
    // Test helpers to set up mock values
    void setAnalogInput(int pin, int value) {
        analogInputs[pin] = value;
    }
    
    int getAnalogOutput(int pin) {
        return analogOutputs[pin];
    }
    
    void setMockMillis(unsigned long value) {
        mockMillis = value;
    }
    
    // IHAL interface implementation
    int analogRead(int pin) override {
        if (analogInputs.find(pin) != analogInputs.end()) {
            return analogInputs[pin];
        }
        return 0;
    }
    
    void analogWrite(int pin, int value) override {
        analogOutputs[pin] = value;
    }
    
    void displayInit() override {}
    void displayClear() override {}
    void displaySetCursor(int x, int y) override {}
    void displayPrint(const char* text) override {}
    void displayShow() override {}
    
    void delay(unsigned long ms) override {
        mockMillis += ms;
    }
    
    unsigned long millis() override {
        return mockMillis;
    }
};

#endif // MOCK_HAL_H
