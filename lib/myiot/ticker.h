#include <Arduino.h>

namespace myiot
{

class Ticker
{
private:
    unsigned long last_run;
    unsigned long count;
    bool is_enabled;
    unsigned long interval;
    std::function<void()> callback;
public:
    Ticker(unsigned long, std::function<void()>);
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