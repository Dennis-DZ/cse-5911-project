#ifndef PWM_OUTPUT_H
#define PWM_OUTPUT_H

#include "../hal/IHAL.h"

/**
 * PWM Output
 * 
 * Controls a PWM output pin. Accepts normalized values (0.0-1.0) and
 * converts them to the appropriate PWM range (0-255 for Arduino).
 */
class PWMOutput {
private:
    IHAL* hal;
    int pin;
    float currentValue;
    static const int PWM_MAX = 255;  // Arduino PWM is 8-bit
    
public:
    /**
     * Constructor
     * @param hal Hardware abstraction layer for PWM control
     * @param pin PWM-capable pin number
     */
    PWMOutput(IHAL* hal, int pin);
    
    /**
     * Set the output value
     * @param value Normalized value between 0.0 and 1.0
     */
    void setValue(float value);
    
    /**
     * Get the current output value
     * @return Current normalized value between 0.0 and 1.0
     */
    float getValue() const;
};

#endif // PWM_OUTPUT_H
