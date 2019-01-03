#include "myiot.h"

namespace myiot
{


Input::Input(const char* n, uint8_t p, uint8_t m = 0) : pin(p), pin_mode(m)
{
    name = (new String(n))->c_str();
    is_sensor = is_button = false;
    value =
      last_read =
      last_publish =
      debounce_ms = 0;
}

void Input::setup()
{
    pinMode(pin, pin_mode);
}

void Input::loop()
{
    // read

    // changed

    // debounce

}


}




