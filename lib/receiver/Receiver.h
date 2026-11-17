#pragma once

#include <Arduino.h>
#include <functional>
#include "LightState.h"

// Listens for LightMessages over ESP-NOW (from any sender, for now).
// Only one Receiver can exist: ESP-NOW has a single receive callback.
class Receiver
{
public:
    using Callback = std::function<void(const LightState &state)>;

    bool begin();  // call once from setup(); false if ESP-NOW failed to start
    void update(); // call on every loop(); fires callbacks
    void onChange(Callback callback);

private:
    LightState last_; // last state passed to onChange_
    bool hasLast_ = false;
    Callback onChange_; // empty until onChange() is called
};
