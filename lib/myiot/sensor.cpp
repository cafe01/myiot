#include "myiot.h"

myiot::Sensor::Sensor(String n, uint8 p)
{
    name = n;
    pin = p;
    is_analog = false;
    read_interval = 1000 * 5;

    reader = [this]() -> String {
        return String(is_analog ? analogRead(pin) : digitalRead(pin));
    };
}

myiot::Sensor::Sensor(String name, std::function<String()> reader)
{
    this->name = name;
    this->reader = reader;

    read_interval = 1000 * 5;
}

myiot::Sensor::~Sensor()
{
}

void myiot::Sensor::setup()
{
    if (pin)
        pinMode(pin, is_pullup ? INPUT_PULLUP : INPUT);
}

bool myiot::Sensor::canRead()
{
    // Serial.printf("millis: %d | last_read: %d\n", millis(), last_read);
    return millis() - last_read > read_interval;
}

String myiot::Sensor::value()
{
    return current_value;
}


String myiot::Sensor::read()
{
    String current_value = reader();

    last_read = millis();

    return current_value;
}
