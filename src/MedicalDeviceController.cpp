#include "MedicalDeviceController.h"

MedicalDeviceController::MedicalDeviceController(IHAL* hal) : hal(hal) {
    // Create input sources
    pot1 = new PotentiometerInput(hal, POT1_PIN);
    pot2 = new PotentiometerInput(hal, POT2_PIN);
    
    // Create outputs
    output1 = new PWMOutput(hal, OUTPUT1_PIN);
    output2 = new PWMOutput(hal, OUTPUT2_PIN);
    
    // Create channels
    channel1 = new Channel(pot1, output1);
    channel2 = new Channel(pot2, output2);
    
    // Create display manager
    display = new DisplayManager(hal);
}

MedicalDeviceController::~MedicalDeviceController() {
    // Clean up in reverse order of creation
    delete display;
    delete channel2;
    delete channel1;
    delete output2;
    delete output1;
    delete pot2;
    delete pot1;
}

void MedicalDeviceController::initialize() {
    display->initialize();
}

void MedicalDeviceController::update() {
    // Update all channels
    channel1->update();
    channel2->update();
    
    // Update display with current values
    display->updateDisplay(
        channel1->getOutputValue(),
        channel2->getOutputValue()
    );
}
