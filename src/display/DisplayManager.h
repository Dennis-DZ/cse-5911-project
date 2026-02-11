#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "../hal/IHAL.h"

/**
 * Display Manager
 * 
 * Manages the display, showing channel output values.
 * Easy to extend with additional information (temperatures, status, etc.)
 */
class DisplayManager {
private:
    IHAL* hal;
    
public:
    /**
     * Constructor
     * @param hal Hardware abstraction layer for display operations
     */
    DisplayManager(IHAL* hal);
    
    /**
     * Initialize the display
     */
    void initialize();
    
    /**
     * Update the display with current channel values
     * @param channel1Value Channel 1 output value (0.0 - 1.0)
     * @param channel2Value Channel 2 output value (0.0 - 1.0)
     */
    void updateDisplay(float channel1Value, float channel2Value);
};

#endif // DISPLAY_MANAGER_H
