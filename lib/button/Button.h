#pragma once

#include <Arduino.h>
#include <functional>

class Button
{
public:
    using Callback = std::function<void()>;

    explicit Button(uint8_t pin);

    void begin();  // call once from setup()
    void update(); // call on every loop(); fires callbacks
    void onPress(Callback callback);

    bool isPressed() const; // true while held down

private:
    const uint8_t pin_;
    int lastReading_ = HIGH;       // HIGH = not pressed (INPUT_PULLUP)
    unsigned long lastChange_ = 0; // millis() of the last accepted change
    Callback onPress_;             // empty until onPress() is called
};