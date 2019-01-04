#include "ticker.h"

namespace myiot
{

Ticker::Ticker(unsigned long i, bool e) : interval(i), is_enabled(e)
{
    last_run = 0;
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
