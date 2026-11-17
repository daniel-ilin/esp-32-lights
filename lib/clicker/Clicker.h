#pragma once

#include <Arduino.h>
#include <array>
#include <functional>
#include "LightController.h"
#include "LightState.h"

// The remote: one set of controls (button + knob) for every registered light.
// Inputs change the selected light; when synced, every other light copies it.
class Clicker
{
public:
    using Callback = std::function<void(size_t index, const LightState &state)>;

    static constexpr size_t MAX_LIGHTS = 20; // ESP-NOW allows up to 20 peers

    void begin(); // call once from setup()

    int addLight(); // registers a light; returns its index, or -1 when full
    size_t lightCount() const;
    const LightState &state(size_t index) const; // index must be < lightCount()

    void handlePress();       // advance to the next mode
    void handleTurn(int dir); // dir is +1 (clockwise) or -1 for one click
    void onChange(Callback callback); // fires once per light that changed

    bool isSynced() const;

private:
    void applyChange(); // after the selected light changed: sync others, notify
    void notify(size_t index);
    void printState() const;

    LightController controller_;
    std::array<LightState, MAX_LIGHTS> lights_;
    size_t lightCount_ = 0;
    size_t selected_ = 0; // the light the controls act on
    bool synced_ = true;  // always synced for now; nothing toggles it yet
    Callback onChange_;   // empty until onChange() is called
};
