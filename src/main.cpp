#include <Arduino.h>
#include "myiot.h"
// #include <Adafruit_Sensor.h>
// #include "DHT.h"

using namespace myiot;

#define LEDPIN 16

Device device("myiot");

// Button btn("btn", 0);

// LED led("led", D2);
LED led("led", D1, D2, D5);

void setup()
{
    // custom config
    // device.addConfig("custom_cfg", 40, "waravalue");

    // btn
    // device.addInput(&btn);

    // inputs
    // auto digital = device.addInput("btn", 0);

    // digital->debounce_ms = 3;

    // digital->onChange([](Input *input) {
    //     Serial.printf("[cb] input %s changed to %d\n", input->getName().c_str(), input->value);
    //     // dev->publishInput(&input, true);
    // });

    // led
    device.addOutput(&led);

    // setup
    led.setup();
    device.setup();

    // cmd
    device.addCommand("led_toggle", [](const char* payload)
    {
        auto led = device.output("led");
        led->toggle();
    });

    device.addTicker(500, []() {
        static int i = 0;
        int turn = ++i % 6;

        if (turn == 0) led.setColor({0, 0, 0});
        else if (turn == 1) led.setColor({255, 0, 0});
        else if (turn == 2) led.setColor({0, 255, 0});
        else if (turn == 3) led.setColor({0, 0, 255});
        else if (turn == 4) led.setColor({ 255, 85, 0});
        else if (turn == 5) led.setColor({255, 255, 255});
    });

}

// int last_loop = 0;

void loop()
{
    device.loop();
}
