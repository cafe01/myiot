#pragma once
#include <Arduino.h>

namespace myiot
{

class Ticker
{
private:
    unsigned long last_run;
    unsigned long count;
    unsigned long interval;
    bool is_enabled;

public:
    std::function<void()> callback;

    Ticker(unsigned long interval, bool is_enabled = true);
    void loop();

    unsigned long getCount() {
        return count;
    }

    void start()
    {
        last_run = 0;
        is_enabled = true;
    }

    void stop()
    {
        is_enabled = false;
    }

    void setInterval(unsigned long i)
    {
        interval = i;
    }
};


}