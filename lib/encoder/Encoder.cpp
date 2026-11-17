#include "Encoder.h"

Encoder::Encoder(uint8_t pinA, uint8_t pinB)
    : pinA_(pinA), pinB_(pinB)
{
}

void Encoder::begin()
{
    pinMode(pinA_, INPUT_PULLUP);
    pinMode(pinB_, INPUT_PULLUP);
    lastA_ = digitalRead(pinA_);
}

void Encoder::update()
{
    int a = digitalRead(pinA_);

    // A just went HIGH → LOW (one click); B tells us the direction
    if (a != lastA_ && a == LOW && onTurn_)
    {
        onTurn_(digitalRead(pinB_) == HIGH ? -1 : +1);
    }

    lastA_ = a;
}

void Encoder::onTurn(Callback callback)
{
    onTurn_ = callback;
}
