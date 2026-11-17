#include "LedLight.h"
#include "color.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LEDC_TIMER LEDC_TIMER_0       // LEDC timer
#define LEDC_MODE LEDC_LOW_SPEED_MODE // LEDC speed mode, low-speed mode
#define LEDC_FREQUENCY 1000           // PWM frequency (1 kHz)

namespace
{
    constexpr ledc_timer_bit_t LEDC_DUTY_RES = LEDC_TIMER_10_BIT;
    constexpr int MAX_DUTY = (1 << LEDC_DUTY_RES) - 1; // 1023
}

LedLight::LedLight(LedOutput redPin, LedOutput greenPin, LedOutput bluePin, bool commonAnode)
    : redPin_(redPin), greenPin_(greenPin), bluePin_(bluePin), commonAnode_(commonAnode)
{
}

void LedLight::begin()
{
    begun_ = true;

    // Configure the LEDC timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,          // Set the speed mode
        .duty_resolution = LEDC_DUTY_RES, // Set the duty cycle resolution
        .timer_num = LEDC_TIMER,          // Select the timer
        .freq_hz = LEDC_FREQUENCY         // Set the PWM frequency
    };
    ledc_timer_config(&ledc_timer); // Apply the timer configuration

    configureLedcChannel(redPin_.pin, redPin_.channel);
    configureLedcChannel(greenPin_.pin, greenPin_.channel);
    configureLedcChannel(bluePin_.pin, bluePin_.channel);

    apply(); // put the LED into its initial state (off)
}

void LedLight::configureLedcChannel(uint8_t pinNum, ledc_channel_t channel)
{
    // Configure the LEDC RED channel
    ledc_channel_config_t ledc_channel = {
        .gpio_num = pinNum,      // GPIO pin for the channel output
        .speed_mode = LEDC_MODE, // Set the speed mode (must match the timer)
        .channel = channel,      // Select the LEDC channel
        .timer_sel = LEDC_TIMER, // Select the timer for this channel to use
        .duty = 0                // Set initial duty cycle to 0
    };
    ledc_channel.flags.output_invert = commonAnode_ ? 1 : 0;

    ledc_channel_config(&ledc_channel); // Apply the channel configuration
}

void LedLight::setOn(bool on)
{
    on_ = on;
    apply();
}

void LedLight::toggle()
{
    setOn(!on_);
}

bool LedLight::isOn() const
{
    return on_;
}

void LedLight::setHue(int hue)
{
    hue_ = (hue % 360 + 360) % 360; // wrap, also for negative values
    apply();
}

int LedLight::hue() const
{
    return hue_;
}

void LedLight::setBrightness(int brightness)
{
    brightness_ = constrain(brightness, 0, 255);
    apply();
}

int LedLight::brightness() const
{
    return brightness_;
}

void LedLight::setState(const LightState &state)
{
    on_ = state.on;
    hue_ = (state.hue % 360 + 360) % 360;
    brightness_ = constrain(state.brightness, 0, 255);
    apply();
}

void LedLight::apply()
{
    if (!begun_)
        return;

    int r = 0, g = 0, b = 0;

    if (on_)
    {
        Color c = hueToRgb(hue_); // 0-255 per color

        // Gamma, computed on the 0-1023 scale for finer dim steps
        long level = (long)brightness_ * brightness_ * MAX_DUTY / (255L * 255);
        r = c.r * level / 255;
        g = c.g * level / 255;
        b = c.b * level / 255;
    }

    writeChannel(redPin_, r); // values are now already 0-1023
    writeChannel(greenPin_, g);
    writeChannel(bluePin_, b);
}

void LedLight::writeChannel(LedOutput output, int duty) // duty is 0-1023
{
    ledc_set_duty(LEDC_MODE, output.channel, duty);
    ledc_update_duty(LEDC_MODE, output.channel);
}