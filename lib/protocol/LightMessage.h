#pragma once

#include <stdint.h>
#include "LightState.h"

// What goes over the air. Fixed-size fields so both boards agree on the bytes.
struct __attribute__((packed)) LightMessage
{
    uint8_t version;
    uint8_t on;
    uint16_t hue;
    uint8_t brightness;
};

const uint8_t LIGHT_MESSAGE_VERSION = 1; // bump when LightMessage changes

inline LightMessage toMessage(const LightState &state)
{
    return {LIGHT_MESSAGE_VERSION,
            state.on,
            (uint16_t)state.hue,
            (uint8_t)state.brightness};
}

inline LightState toState(const LightMessage &message)
{
    LightState state;
    state.on = message.on;
    state.hue = message.hue;
    state.brightness = message.brightness;
    return state;
}
