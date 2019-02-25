#include "myiot.h"


namespace myiot
{

    LED::LED(const char* name, uint8_t pin) : Output(name , pin)
    {

    }

    LED::LED(const char* name, uint8_t r_pin, uint8_t g_pin, uint8_t b_pin) : Output(name), r_pin(r_pin), g_pin(g_pin), b_pin(b_pin)
    {
        is_rgb = true;
    }

    void LED::setup()
    {
        if (is_rgb)
        {
            pinMode(r_pin, OUTPUT);
            pinMode(g_pin, OUTPUT);
            pinMode(b_pin, OUTPUT);
            Serial.printf("[LED] rgb pins: %d, %d, %d\n", r_pin, g_pin, b_pin);
        }
        else
            pinMode(pin, OUTPUT);
    }

    void LED::setColor(Color new_color, bool apply_color /* = true */) {
        color = new_color;
        Serial.printf("[LED] setColor(%d, %d, %d)\n", color.red, color.green, color.blue);

        if (apply_color)
            applyColor();
    }

    void LED::applyColor() {
        int r_pwm = map(color.red, 0, 255, 0, PWMRANGE);
        int g_pwm = map(color.green, 0, 255, 0, PWMRANGE);
        int b_pwm = map(color.blue, 0, 255, 0, PWMRANGE);
        analogWrite(r_pin, is_anode ? PWMRANGE - r_pwm : r_pwm);
        analogWrite(g_pin, is_anode ? PWMRANGE - g_pwm : g_pwm);
        analogWrite(b_pin, is_anode ? PWMRANGE - b_pwm : b_pwm);
    }
}