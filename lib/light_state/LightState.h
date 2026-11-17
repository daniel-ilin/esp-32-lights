#pragma once

// What a light should look like. Plain data, so it can be copied and sent around.
struct LightState
{
    bool on = false;
    int hue = 0;          // 0-359
    int brightness = 255; // 0-255
};
