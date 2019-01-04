#include "myiot.h"

namespace myiot
{


Input::Input(const char* n, uint8_t p, uint8_t m = 0) : pin(p), pin_mode(m)
{
    // copy hardcoded char into string
    this->name = (new String(n))->c_str();

    // init
    is_enabled = true;
    is_sensor = is_button = false;
    debounce_ms = 10;
    value =
      last_read =
      last_publish = 0;

}

void Input::setup()
{
    pinMode(pin, pin_mode);

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
                return String(read_float());
            else
                return String(read_int());
        };
    }

    // onchange
    if (!on_change)
    {
        Serial.printf("[WARNING] input %s has no on_change\n", name);
    }

}

void Input::loop()
{
    if (read_float)
        // loop_int();
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
    if (read_value == *value)
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
    last_read = now;
    *value = read_value;

    Serial.printf("[Input] %s = %.2f\n", name, (float)read_value);

    // trigger on_change
    if (this->on_change)
    {
        // Serial.println("calling input on_change");
        on_change(*this);
    }
}


}




