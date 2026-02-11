#ifndef ARDUINO_HAL_H
#define ARDUINO_HAL_H

#include "IHAL.h"

#ifndef UNIT_TEST
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

/**
 * Arduino implementation of the Hardware Abstraction Layer
 * 
 * This class provides Arduino-specific implementations of all hardware
 * operations defined in the IHAL interface.
 */
class ArduinoHAL : public IHAL {
private:
#ifndef UNIT_TEST
    Adafruit_SSD1306* display;
#endif
    
public:
    ArduinoHAL();
    ~ArduinoHAL();
    
    // Analog I/O
    int analogRead(int pin) override;
    void analogWrite(int pin, int value) override;
    
    // Display operations
    void displayInit() override;
    void displayClear() override;
    void displaySetCursor(int x, int y) override;
    void displayPrint(const char* text) override;
    void displayShow() override;
    
    // Timing
    void delay(unsigned long ms) override;
    unsigned long millis() override;
};

#endif // ARDUINO_HAL_H
