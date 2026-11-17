#pragma once

#include <Arduino.h>
#include <functional>

class Encoder
{
public:
    using Callback = std::function<void(int dir)>; // dir is +1 (clockwise) or -1

    Encoder(uint8_t pinA, uint8_t pinB);

    void begin();  // call once from setup()
    void update(); // call on every loop(); fires callbacks
    void onTurn(Callback callback);

private:
    const uint8_t pinA_;
    const uint8_t pinB_;
    int lastA_ = HIGH; // HIGH = idle (INPUT_PULLUP)
    Callback onTurn_;  // empty until onTurn() is called
};
