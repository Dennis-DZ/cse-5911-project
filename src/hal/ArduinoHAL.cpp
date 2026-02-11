#include "ArduinoHAL.h"

#ifndef UNIT_TEST

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

ArduinoHAL::ArduinoHAL() {
    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

ArduinoHAL::~ArduinoHAL() {
    delete display;
}

int ArduinoHAL::analogRead(int pin) {
    return ::analogRead(pin);
}

void ArduinoHAL::analogWrite(int pin, int value) {
    ::analogWrite(pin, value);
}

void ArduinoHAL::displayInit() {
    if (!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        // Display initialization failed
        // In a production system, you might want to handle this more gracefully
        for(;;); // Halt
    }
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->display();
}

void ArduinoHAL::displayClear() {
    display->clearDisplay();
}

void ArduinoHAL::displaySetCursor(int x, int y) {
    display->setCursor(x, y);
}

void ArduinoHAL::displayPrint(const char* text) {
    display->print(text);
}

void ArduinoHAL::displayShow() {
    display->display();
}

void ArduinoHAL::delay(unsigned long ms) {
    ::delay(ms);
}

unsigned long ArduinoHAL::millis() {
    return ::millis();
}

#else

// Stub implementation for unit testing
ArduinoHAL::ArduinoHAL() {}
ArduinoHAL::~ArduinoHAL() {}
int ArduinoHAL::analogRead(int pin) { return 0; }
void ArduinoHAL::analogWrite(int pin, int value) {}
void ArduinoHAL::displayInit() {}
void ArduinoHAL::displayClear() {}
void ArduinoHAL::displaySetCursor(int x, int y) {}
void ArduinoHAL::displayPrint(const char* text) {}
void ArduinoHAL::displayShow() {}
void ArduinoHAL::delay(unsigned long ms) {}
unsigned long ArduinoHAL::millis() { return 0; }

#endif
