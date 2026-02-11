#ifndef IHAL_H
#define IHAL_H

/**
 * Hardware Abstraction Layer Interface
 * 
 * This interface defines all hardware operations needed by the application.
 * Different platforms (Arduino, Raspberry Pi, etc.) implement this interface
 * to provide platform-specific functionality.
 */
class IHAL {
public:
    virtual ~IHAL() {}
    
    // Analog I/O
    virtual int analogRead(int pin) = 0;
    virtual void analogWrite(int pin, int value) = 0;
    
    // Display operations
    virtual void displayInit() = 0;
    virtual void displayClear() = 0;
    virtual void displaySetCursor(int x, int y) = 0;
    virtual void displayPrint(const char* text) = 0;
    virtual void displayShow() = 0;  // For displays that need explicit refresh
    
    // Timing
    virtual void delay(unsigned long ms) = 0;
    virtual unsigned long millis() = 0;
};

#endif // IHAL_H
