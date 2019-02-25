#pragma once
#include <Arduino.h>

namespace myiot
{

class Output
{
  protected:
    uint8_t pin;
    int value;
  public:
    char *name;
    bool is_analog = false;
    Output(const char *n);
    Output(const char *n, uint8_t p);

    virtual void setup();

    virtual void loop() {}

    virtual void write(int value);

    int pinValue()
    {
        return is_analog ? analogRead(pin) : digitalRead(pin);
    }

    void toggle()
    {
        if (!is_analog)
          write(!pinValue());
    }

    // TODO onChange()
};

} // namespace myiot