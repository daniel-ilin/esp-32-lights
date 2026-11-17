#pragma once

#include <Arduino.h>
#include "LightState.h"
#include "driver/ledc.h"

struct LedOutput
{
    uint8_t pin;
    ledc_channel_t channel;
};

// An RGB LED with on/off, hue and brightness.
// Every setter immediately updates the LED, so the hardware always matches the state.
class LedLight
{
public:
    LedLight(LedOutput redPin, LedOutput greenPin, LedOutput bluePin, bool commonAnode);

    void begin(); // call once from setup()

    void setOn(bool on);
    void toggle();
    bool isOn() const;

    void setHue(int hue); // any value; wraps into 0-359
    int hue() const;

    void setBrightness(int brightness); // clamped to 0-255
    int brightness() const;

    void setState(const LightState &state); // on, hue and brightness in one update

private:
    void apply(); // write the current state to the pins
    void writeChannel(LedOutput output, int duty);

    void configureLedcChannel(uint8_t pinNum, ledc_channel_t channel);

    const LedOutput redPin_;
    const LedOutput greenPin_;
    const LedOutput bluePin_;
    const bool commonAnode_;

    bool on_ = false;
    int hue_ = 0;
    int brightness_ = 255;
    bool begun_ = false;
};