#include <Arduino.h>
#include "LedLight.h"
#include "Button.h"
#include "Encoder.h"
#include "Clicker.h"
#include "Broadcaster.h"

// LED pins
const uint8_t RED_PIN = 25;
const uint8_t GREEN_PIN = 26;
const uint8_t BLUE_PIN = 27;

// Encoder pins
const uint8_t BTN_PIN = 14;
const uint8_t PIN_A = 4;
const uint8_t PIN_B = 18;

LedLight light(RED_PIN, GREEN_PIN, BLUE_PIN, true); // true = common anode
Button button(BTN_PIN);
Encoder knob(PIN_A, PIN_B);
Clicker clicker;
Broadcaster broadcaster;

void setup()
{
  Serial.begin(115200);

  if (!broadcaster.begin())
  {
    Serial.println("ESP-NOW failed to start; updates won't be broadcast");
  }

  int index = clicker.addLight(); // the one LED on this board
  clicker.begin();
  clicker.onChange([](size_t, const LightState &state)
                   {
                     light.setState(state);   // update our own LED
                     broadcaster.send(state); // and every listening bulb
                   });

  light.begin(); // sets up the LED (starts off)
  light.setState(clicker.state(index));

  button.begin();
  button.onPress([]
                 { clicker.handlePress(); });

  knob.begin();
  knob.onTurn([](int dir)
              { clicker.handleTurn(dir); });

  broadcaster.send(clicker.state(index));
}

void loop()
{
  button.update(); // calls clicker.handlePress() when pressed
  knob.update();   // calls clicker.handleTurn() on each click
  delay(1);        // crude debounce
}
