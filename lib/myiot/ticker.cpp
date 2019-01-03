#include "ticker.h"

namespace myiot
{

Ticker::Ticker(unsigned long i, std::function<void()> c) : interval(i), callback(c)
{
    stop();
}

void Ticker::loop()
{
    if (!is_enabled) return;
    if ( millis() - last_run < interval) return;

    last_run = millis();
    ++count;
    callback();
}

}
