#include <Arduino.h>
#include "LedLight.h"
#include "Receiver.h"

// LED pins
const uint8_t RED_PIN = 25;
const ledc_channel_t RED_CHANNEL = LEDC_CHANNEL_0;

const uint8_t GREEN_PIN = 26;
const ledc_channel_t GREEN_CHANNEL = LEDC_CHANNEL_1;

const uint8_t BLUE_PIN = 27;
const ledc_channel_t BLUE_CHANNEL = LEDC_CHANNEL_2;

LedLight light({RED_PIN, RED_CHANNEL}, {GREEN_PIN, GREEN_CHANNEL}, {BLUE_PIN, BLUE_CHANNEL}, true); // true = common anode
Receiver receiver;

void setup()
{
  Serial.begin(115200);

  light.begin(); // sets up the LED (starts off)

  if (!receiver.begin())
  {
    Serial.println("ESP-NOW failed to start; no updates will arrive");
  }

  receiver.onChange([](const LightState &state)
                    {
                      light.setState(state);
                      Serial.printf("%-3s hue %3d  brightness %3d\n",
                                    state.on ? "ON" : "OFF", state.hue, state.brightness); });
}

void loop()
{
  receiver.update(); // calls the onChange handler when a new state arrives
}
