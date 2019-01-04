#pragma once
#include <Arduino.h>
#include "input.h"

namespace myiot
{


class Button : public Input
{
  private:
    /* data */
    // String name;
    // uint8_t pin;
    // uint8_t input_mode;
    // bool is_pullup;
    // // bool is_internal_pullup;
    // std::map<uint8_t, std::function<void()>> click_callbacks;
    // bool is_down;
    // unsigned long last_down;
    // unsigned long last_up;

    // uint8_t pending_clicks;
  public:
    Button(const char* name, uint8_t pin) : Input(name, pin)
    {
        is_button = true;
    }
    // void setup();
    // void onClicks(uint8_t number_of_clicks, std::function<void()> callback);
    // void loop();
};


}