#include <Arduino.h>
#include "Button.h"
#include "Encoder.h"
#include "Clicker.h"
#include "Broadcaster.h"

// Encoder pins
const uint8_t BTN_PIN = 14;
const uint8_t KNOB_PIN_A = 4;
const uint8_t KNOB_PIN_B = 18;

Button button(BTN_PIN);
Encoder knob(KNOB_PIN_A, KNOB_PIN_B);
Clicker clicker;
Broadcaster broadcaster;

void setup()
{
  Serial.begin(115200);

  if (!broadcaster.begin())
  {
    Serial.println("ESP-NOW failed to start; updates won't be broadcast");
  }

  int index = clicker.addLight(); // one light: a broadcast reaches every bulb
  clicker.begin();
  clicker.onChange([](size_t, const LightState &state)
                   { broadcaster.send(state); });

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
