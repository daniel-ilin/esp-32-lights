#include "LightController.h"

namespace
{
    // Knob settings
    const int CLICKS_PER_REV = 20; // EC11 detents per full turn
    const int ROTATIONS = 2;       // full turns to sweep each setting
    const int TOTAL_CLICKS = CLICKS_PER_REV * ROTATIONS;

    const int HUE_STEP = 360 / TOTAL_CLICKS; // degrees per click
    const int MIN_BRIGHTNESS = 17;           // dimmest visible level
}

void LightController::handlePress(LightState &state)
{
    mode_ = nextMode(mode_);
    state.on = mode_ != Mode::OFF;
}

bool LightController::handleTurn(LightState &state, int dir)
{
    switch (mode_)
    {
    case Mode::OFF:
        return false; // knob does nothing while off

    case Mode::BRIGHTNESS:
    {
        int level = brightnessToLevel(state.brightness);
        int next = constrain(level + dir, 1, TOTAL_CLICKS);
        if (next == level)
        {
            return false; // already at the end of the range
        }
        state.brightness = levelToBrightness(next);
        return true;
    }

    case Mode::HUE:
        state.hue = (state.hue + dir * HUE_STEP + 360) % 360; // wrap around 360
        return true;
    }
    return false;
}

LightController::Mode LightController::mode() const
{
    return mode_;
}

LightController::Mode LightController::nextMode(Mode m)
{
    switch (m)
    {
    case Mode::OFF:
        return Mode::BRIGHTNESS;
    case Mode::BRIGHTNESS:
        return Mode::HUE;
    case Mode::HUE:
        return Mode::OFF;
    }
    return Mode::OFF;
}

const char *LightController::modeName(Mode m)
{
    switch (m)
    {
    case Mode::OFF:
        return "OFF";
    case Mode::BRIGHTNESS:
        return "BRIGHTNESS";
    case Mode::HUE:
        return "HUE";
    }
    return "?";
}

// Map knob level 1..TOTAL_CLICKS to MIN_BRIGHTNESS..255
int LightController::levelToBrightness(int level)
{
    return MIN_BRIGHTNESS + (level - 1) * (255 - MIN_BRIGHTNESS) / (TOTAL_CLICKS - 1);
}

// Inverse of levelToBrightness(): the nearest knob level for a brightness
int LightController::brightnessToLevel(int brightness)
{
    int range = 255 - MIN_BRIGHTNESS;
    int level = ((brightness - MIN_BRIGHTNESS) * (TOTAL_CLICKS - 1) + range / 2) / range + 1;
    return constrain(level, 1, TOTAL_CLICKS);
}
