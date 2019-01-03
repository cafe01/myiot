#include "myiot.h"


#define BUTTON_DEBOUNCE_TIME 100
#define BUTTON_HOLD_TIME 2000
#define BUTTON_CLICK_TIME 500

namespace myiot
{




// void Button::loop()
// {

//     // pressed
//     if (digitalRead(pin) == LOW)
//     {
//         // debounce
//         auto now = millis();
//         if (now - last_down < BUTTON_DEBOUNCE_TIME) return;

//         // change state
//         if (!is_down)
//         {
//             is_down = true;
//             last_down = now;
//             // Serial.printf("DOWN\n");
//         }

//         // detect long press
//         if (now - last_down > (BUTTON_HOLD_TIME - BUTTON_DEBOUNCE_TIME))
//         {
//             Serial.println("long press");



//             auto it = click_callbacks.find(0);
//             if (it != click_callbacks.end())
//                 it->second();
//         }
//     }
//     else

//     // unpressed
//     {
//         // debounce
//         auto now = millis();
//         if (now - last_up < BUTTON_DEBOUNCE_TIME) return;

//         // current state is up
//         if (!is_down)
//         {
//             // is idle
//             if (pending_clicks == 0) return;

//             // interclick wait
//             if (millis() - last_up < BUTTON_CLICK_TIME) return;

//             // dispatch
//             // Serial.printf("=> UP %d %d\n", (int)pending_clicks, (int)(millis() - last_up));
//             auto it = click_callbacks.find(pending_clicks);
//             if (it != click_callbacks.end())
//             {
//                 Serial.printf("[Button] running %d click handler\n", (int)pending_clicks);
//                 it->second();
//             }
//             else
//             {
//                 Serial.printf("[Button] no %d click handler registered\n", (int)pending_clicks);
//             }

//             pending_clicks = 0;
//             last_up = 0;
//         }

//         // current state is down
//         else
//         {
//             // change state
//             is_down = false;
//             last_down = 0;
//             pending_clicks++;
//             // Serial.printf("UP %d %d\n", (int)pending_clicks, (int)(millis() - last_up));
//             last_up = millis();
//         }

//     }
// }

// void Button::onClicks(uint8_t number_of_clicks, std::function<void()> callback)
// {
//     // make room

//     // if (click_callbacks.capacity() < (number_of_clicks + 1U))
//     // {
//     //     click_callbacks.reserve(number_of_clicks + 1U);
//     // }

//     // // TODO exception if already registered

//     // // register
//     // click_callbacks[number_of_clicks] = callback;
//     click_callbacks.insert(std::pair<uint8_t, std::function<void()>>(number_of_clicks, callback));

//     //
//     // Serial.printf("click_callbacks capacity=%d size=%d\n", click_callbacks.capacity(), click_callbacks.size());
// }


} // namespace