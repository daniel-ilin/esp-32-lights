#include "Receiver.h"
#include <WiFi.h>
#include <esp_now.h>
#include <esp_arduino_version.h>
#include "LightMessage.h"

namespace
{
    // Written by the Wi-Fi task, read by loop(); guarded by lock
    portMUX_TYPE lock = portMUX_INITIALIZER_UNLOCKED;
    LightMessage pending;
    bool hasPending = false;

    void handlePacket(const uint8_t *data, int len)
    {
        if (len != sizeof(LightMessage))
        {
            return; // not one of ours
        }

        LightMessage message;
        memcpy(&message, data, sizeof(message));
        if (message.version != LIGHT_MESSAGE_VERSION)
        {
            return;
        }

        portENTER_CRITICAL(&lock);
        pending = message;
        hasPending = true;
        portEXIT_CRITICAL(&lock);
    }

    // Runs on the Wi-Fi task, not in loop(): only store the packet here.
    // The callback's signature changed in Arduino core 3.
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    void onReceive(const esp_now_recv_info_t *, const uint8_t *data, int len)
#else
    void onReceive(const uint8_t *, const uint8_t *data, int len)
#endif
    {
        handlePacket(data, len);
    }

    bool sameState(const LightState &a, const LightState &b)
    {
        return a.on == b.on && a.hue == b.hue && a.brightness == b.brightness;
    }
}

bool Receiver::begin()
{
    WiFi.mode(WIFI_STA); // ESP-NOW needs the radio on, but no network
    if (esp_now_init() != ESP_OK)
    {
        return false;
    }
    esp_now_register_recv_cb(onReceive);
    return true;
}

void Receiver::update()
{
    portENTER_CRITICAL(&lock);
    bool received = hasPending;
    LightMessage message = pending;
    hasPending = false;
    portEXIT_CRITICAL(&lock);

    if (!received)
    {
        return;
    }

    LightState state = toState(message);
    if (hasLast_ && sameState(state, last_))
    {
        return; // nothing changed
    }

    last_ = state;
    hasLast_ = true;
    if (onChange_)
    {
        onChange_(state);
    }
}

void Receiver::onChange(Callback callback)
{
    onChange_ = callback;
}
