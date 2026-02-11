#ifndef MEDICAL_DEVICE_CONTROLLER_H
#define MEDICAL_DEVICE_CONTROLLER_H

#include "hal/IHAL.h"
#include "inputs/PotentiometerInput.h"
#include "outputs/PWMOutput.h"
#include "control/Channel.h"
#include "display/DisplayManager.h"

/**
 * Medical Device Controller
 * 
 * Main controller for the medical device. Orchestrates all channels
 * and the display. This is the platform-independent application logic.
 */
class MedicalDeviceController {
private:
    IHAL* hal;
    
    // Input sources
    PotentiometerInput* pot1;
    PotentiometerInput* pot2;
    
    // Outputs
    PWMOutput* output1;
    PWMOutput* output2;
    
    // Channels
    Channel* channel1;
    Channel* channel2;
    
    // Display
    DisplayManager* display;
    
    // Pin configuration
    static const int POT1_PIN = 0;      // A0
    static const int POT2_PIN = 1;      // A1
    static const int OUTPUT1_PIN = 9;   // PWM pin 9
    static const int OUTPUT2_PIN = 10;  // PWM pin 10
    
public:
    /**
     * Constructor
     * @param hal Hardware abstraction layer
     */
    MedicalDeviceController(IHAL* hal);
    
    /**
     * Destructor - clean up resources
     */
    ~MedicalDeviceController();
    
    /**
     * Initialize the controller and all subsystems
     */
    void initialize();
    
    /**
     * Update all channels and display - call this in main loop
     */
    void update();
};

#endif // MEDICAL_DEVICE_CONTROLLER_H
