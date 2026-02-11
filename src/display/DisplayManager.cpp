#include "DisplayManager.h"

DisplayManager::DisplayManager(IHAL* hal) : hal(hal) {
}

void DisplayManager::initialize() {
    hal->displayInit();
    hal->displayClear();
    hal->displayShow();
}

void DisplayManager::updateDisplay(float channel1Value, float channel2Value) {
    hal->displayClear();
    
    // Display Channel 1 voltage (as percentage)
    hal->displaySetCursor(0, 0);
    hal->displayPrint("Channel 1:");
    hal->displaySetCursor(0, 10);
    char buffer[16];
    // Convert to percentage
    int percent1 = (int)(channel1Value * 100);
    sprintf(buffer, "%d%%", percent1);
    hal->displayPrint(buffer);
    
    // Display Channel 2 voltage (as percentage)
    hal->displaySetCursor(0, 30);
    hal->displayPrint("Channel 2:");
    hal->displaySetCursor(0, 40);
    int percent2 = (int)(channel2Value * 100);
    sprintf(buffer, "%d%%", percent2);
    hal->displayPrint(buffer);
    
    hal->displayShow();
}
