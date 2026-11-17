#pragma once

#include <Arduino.h>
#include "LightState.h"

// Sends LightStates over ESP-NOW to every listening board (broadcast).
class Broadcaster
{
public:
    bool begin();                        // call once from setup(); false if ESP-NOW failed to start
    bool send(const LightState &state);  // false if the packet couldn't be queued
};
