#pragma once
#include <Arduino.h>

namespace myiot
{

class Output
{
  protected:
    uint8_t pin;

  public:
    char *name;
    bool is_analog;
    Output(const char *n, uint8_t p)
    {
        // name
        size_t size = strlen(n) + 1;
        this->name = new char[size];
        strncpy(name, n, size);

        //
        pin = p;
        is_analog = false;
    }

    virtual void setup()
    {
        pinMode(pin, OUTPUT);
    }

    virtual void loop() {}

    virtual void write(int value);

    int pinValue()
    {
        return is_analog ? analogRead(pin) : digitalRead(pin);
    }

    void toggle()
    {
        write(!pinValue());
    }
};

} // namespace myiot