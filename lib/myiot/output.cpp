#include "myiot.h"

namespace myiot
{

    Output::Output(const char *n)
    {
        // name
        size_t size = strlen(n) + 1;
        this->name = new char[size];
        strncpy(name, n, size);
    }

    Output::Output(const char *n, uint8_t p) : pin(p)
    {
        // name
        size_t size = strlen(n) + 1;
        this->name = new char[size];
        strncpy(name, n, size);
    }

    void Output::setup()
    {
        if (!isnan(pin))
            pinMode(pin, OUTPUT);

        // value = is_analog ? analogRead(pin) : digitalRead(pin);
    }

    void Output::write(int value)
    {
        if (is_analog)
            analogWrite(pin, value);
        else
            digitalWrite(pin, (uint8_t)value);
    }
}