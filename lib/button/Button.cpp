#include "Button.h"

namespace
{
    const unsigned long DEBOUNCE_MS = 50;
}

Button::Button(uint8_t pin)
    : pin_(pin)
{
}

void Button::begin()
{
    pinMode(pin_, INPUT_PULLUP);
    lastReading_ = digitalRead(pin_);
}

void Button::update()
{
    int reading = digitalRead(pin_);

    // Ignore changes within DEBOUNCE_MS of the last one (contact bounce)
    if (reading != lastReading_ && millis() - lastChange_ > DEBOUNCE_MS)
    {
        lastChange_ = millis();
        lastReading_ = reading;

        if (reading == LOW && onPress_) // pressed, and a callback is registered
        {
            onPress_();
        }
    }
}

void Button::onPress(Callback callback)
{
    onPress_ = callback;
}

bool Button::isPressed() const
{
    return digitalRead(pin_) == LOW;
}