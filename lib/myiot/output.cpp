#include "myiot.h"

namespace myiot
{

    void Output::write(int value)
    {
        if (is_analog)
            analogWrite(pin, value);
        else
            digitalWrite(pin, (uint8_t)value);
    }
}