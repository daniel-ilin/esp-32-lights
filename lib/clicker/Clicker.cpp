#include "Clicker.h"
#include "LightController.h"

namespace
{
    const unsigned long DEBOUNCE_MS = 50;
}

void Clicker::begin()
{
}

int Clicker::addLight()
{
    if (lightCount_ == MAX_LIGHTS)
    {
        return -1;
    }

    size_t index = lightCount_++;
    if (synced_ && index > 0)
    {
        lights_[index] = lights_[selected_]; // join in the current look
    }
    return index;
}

size_t Clicker::lightCount() const
{
    return lightCount_;
}

const LightState &Clicker::state(size_t index) const
{
    return lights_[index];
}

void Clicker::handlePress()
{
    if (lightCount_ == 0)
    {
        return;
    }
    controller_.handlePress(lights_[selected_]);
    applyChange();
}

void Clicker::handleTurn(int dir)
{
    if (lightCount_ == 0)
    {
        return;
    }
    if (controller_.handleTurn(lights_[selected_], dir))
    {
        applyChange();
    }
}

void Clicker::onChange(Callback callback)
{
    onChange_ = callback;
}

bool Clicker::isSynced() const
{
    return synced_;
}

void Clicker::applyChange()
{
    if (synced_)
    {
        for (size_t i = 0; i < lightCount_; i++)
        {
            lights_[i] = lights_[selected_];
            notify(i);
        }
    }
    else
    {
        notify(selected_);
    }
    printState();
}

void Clicker::notify(size_t index)
{
    if (onChange_)
    {
        onChange_(index, lights_[index]);
    }
}

void Clicker::printState() const
{
    const LightState &s = lights_[selected_];
    Serial.printf("%-10s hue %3d  brightness %3d  (light %u of %u%s)\n",
                  LightController::modeName(controller_.mode()), s.hue, s.brightness,
                  selected_ + 1, lightCount_, synced_ ? ", synced" : "");
}
