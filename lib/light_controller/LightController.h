#pragma once

#include <Arduino.h>
#include "LightState.h"

// The rules: how a button press or knob turn changes a light's state.
// Holds the current mode (one knob, one mode), but no light state of its own;
// the caller passes in the LightState to change.
class LightController
{
public:
    enum class Mode
    {
        OFF,
        BRIGHTNESS,
        HUE
    };

    void handlePress(LightState &state);           // advance to the next mode
    bool handleTurn(LightState &state, int dir);   // dir is +1/-1; returns true if state changed

    Mode mode() const;
    static const char *modeName(Mode m);

private:
    static Mode nextMode(Mode m);
    static int levelToBrightness(int level);
    static int brightnessToLevel(int brightness);

    Mode mode_ = Mode::OFF;
};
