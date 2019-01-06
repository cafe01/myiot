#pragma once
#include <Arduino.h>

namespace myiot
{

class Output
{
protected:

    const char* name;
    uint8_t pin;

public:

    Output(const char* n, uint8_t p)
    {

    }

    virtual void setup()
    {

    }

    virtual void loop();
};

}