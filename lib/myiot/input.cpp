#include "myiot.h"

namespace myiot
{


Input::Input(const String &n) : name(n)
{
}

Input::Input(const String &n, uint8_t p) : name(n), pin(p)
{
}

void Input::setup()
{
    // default read_int
    if (!(read_int || read_float))
    {
        if (is_analog)
            read_int = [this]() -> int { return analogRead(pin); };
        else
            read_int = [this]() -> int { return digitalRead(pin); };
    }

    // default to_string
    if (!to_string)
    {
        to_string = [this]() -> String {
            if (read_float)
                return String(float_value);
            else
                return String(value);
        };
    }
}

void Input::loop()
{
    // warming up
    if (millis() < warmup_time) return;

    // read interval
    if (millis() - last_read < read_interval) return;

    // read as float preferably, or as int
    if (read_float)
        _loop(&float_value, read_float);
    else
        _loop(&value, read_int);

}

template<typename T> void Input::_loop(T* value, std::function<T()> reader)
{
    auto read_value = reader();

    // NaN
    if (isnan(read_value)) return;

    auto now = millis();

    // not changed
    if (last_read > 0 && read_value == *value)
    {
        last_read = now;
        return;
    }

    // debounce
    if (now - last_read < debounce_ms)
    {
        return;
    }

    // change state
    *value = read_value;

    Serial.printf("[Input] %s = %s\n", name.c_str(), String(read_value).c_str());

    // trigger on_change
    for (size_t i = 0; i < on_change.size(); ++i)
    {
        // Serial.printf("[Input:%s] calling callback %d \n", name.c_str(), (int)i);
        on_change[i](this);
    }

    // change times
    last_change = last_read = now;
}


}




