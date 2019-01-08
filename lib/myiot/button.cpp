#include "myiot.h"


#define BUTTON_HOLD_TIME 2000
#define BUTTON_CLICK_TIME 350

namespace myiot
{

Button::Button(const char* name, uint8_t pin) : Input(name, pin)
{
    is_button = true;
}

void Button::setup()
{
    Input::setup();

    onChange([this](Input* me) {
        on_state_change();
    });
}

void Button::on_state_change()
{
    // down
    if (value == LOW)
    {
        pending_clicks++;
        // return;
    }

    // up
    // auto now = millis();

    // // inter-click
    // if (now - last_change < BUTTON_CLICK_TIME)
    // {
    //     return;
    // }

    // idle
    // if (pending_clicks == 0) return;

}

void Button::loop()
{
    Input::loop();

    // idle
    if (pending_clicks == 0 ) return;

    // wait next click
    if (millis() - last_change < BUTTON_CLICK_TIME) return;

    // longpress
    if (value == LOW)
    {
        if (pending_clicks == 1 && (millis() - last_change > BUTTON_HOLD_TIME))
        {
            triggerClicks(0);
        }
        return;
    }

    // dispatch click handler when btn is UP
    // Serial.printf("[Button] %d click(s) detected. (%d ms)\n", pending_clicks, millis() - last_change);
    triggerClicks(pending_clicks);
}


void Button::onClicks(uint8_t number_of_clicks, std::function<void()> callback)
{
    click_callbacks.insert(std::pair<uint8_t, std::function<void()>>(number_of_clicks, callback));
}

void Button::triggerClicks(uint8_t number_of_clicks, bool reset_pending_clicks)
{
    auto it = click_callbacks.find(number_of_clicks);

    if (it != click_callbacks.end())
    {
        Serial.printf("[Button] running %d click handler\n", (int)number_of_clicks);
        it->second();
    }
    else
    {
        Serial.printf("[Button] no %d click handler registered\n", (int)number_of_clicks);
    }

    if (reset_pending_clicks)
        pending_clicks = 0;
}


} // namespace