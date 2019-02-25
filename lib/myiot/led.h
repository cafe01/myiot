#pragma once
#include <Arduino.h>
#include "output.h"

#define COLOR_YELLOW { 255, 255, 0 }

namespace myiot
{

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class LED : public Output
{
  private:
    uint8_t r_pin;
    uint8_t g_pin;
    uint8_t b_pin;
    Color color = {0,0,0};
    bool is_rgb = false;

  public:
    bool is_anode = true;
    LED(const char* name, uint8_t pin);
    LED(const char* name, uint8_t r_pin, uint8_t g_pin, uint8_t b_pin);
    void setup();
    // void loop() {}


    void setColor(Color new_color, bool apply_color = true);
    void applyColor();
};


}