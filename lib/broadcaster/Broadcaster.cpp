#include "Broadcaster.h"
#include <WiFi.h>
#include <esp_now.h>
#include "LightMessage.h"

namespace
{
    // The special "everyone" address
    const uint8_t BROADCAST_ADDRESS[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
}

bool Broadcaster::begin()
{
    WiFi.mode(WIFI_STA); // ESP-NOW needs the radio on, but no network
    if (esp_now_init() != ESP_OK)
    {
        return false;
    }

    // ESP-NOW only sends to registered peers, so register "everyone"
    esp_now_peer_info_t peer = {};
    memcpy(peer.peer_addr, BROADCAST_ADDRESS, sizeof(BROADCAST_ADDRESS));
    peer.channel = 0; // 0 = whatever channel the radio is on
    peer.encrypt = false;
    return esp_now_add_peer(&peer) == ESP_OK;
}

bool Broadcaster::send(const LightState &state)
{
    LightMessage message = toMessage(state);
    return esp_now_send(BROADCAST_ADDRESS, (const uint8_t *)&message, sizeof(message)) == ESP_OK;
}
