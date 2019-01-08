#include <Arduino.h>
#include "myiot.h"
// #include <Adafruit_Sensor.h>
// #include "DHT.h"

using namespace myiot;

#define LEDPIN 16

Device device("myiot");

Button btn("btn", 0);

void setup()
{
    // custom config
    // device.addConfig("custom_cfg", 40, "waravalue");

    // btn
    device.addInput(&btn);

    // inputs
    // auto digital = device.addInput("btn", 0);

    // digital->debounce_ms = 3;

    // digital->onChange([](Input *input) {
    //     Serial.printf("[cb] input %s changed to %d\n", input->getName().c_str(), input->value);
    //     // dev->publishInput(&input, true);
    // });

    // outputs
    auto led = device.addOutput("led", LEDPIN);
    led->write(1); // start off

    // setup
    device.setup();

    // cmd
    device.addCommand("led_toggle", [](const char* payload)
    {
        auto led = device.output("led");
        led->toggle();
    });

    // device.addTicker(1000, []() {
    //     device.runCommand("report");
    // });

}

// int last_loop = 0;

void loop()
{
    device.loop();
}
