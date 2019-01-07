#include <Arduino.h>
#include "myiot.h"
// #include <Adafruit_Sensor.h>
// #include "DHT.h"

using namespace myiot;

#define LEDPIN 16

Device device("myiot");


void setup()
{
    // custom config
    // device.addConfig("custom_cfg", 40, "waravalue");

    // inputs
    auto digital = device.addInput("btn", 0);

    digital->debounce_ms = 3;

    digital->onChange([](Input *input) {
        Serial.printf("[cb] input %s changed to %d\n", input->getName().c_str(), input->value);
        // dev->publishInput(&input, true);
    });

    // outputs
    auto led = device.addOutput("led", LEDPIN);
    led->write(1); // start off

    // setup
    device.setup();

    // cmd
    device.addCommand("led_toggle", [](const char* payload)
    {
        auto led = device.output("led");
        led->write(!led->pinValue());
    });

    // device.addTicker(1000, []() {
    //     device.runCommand("report");
    // });

}

void loop()
{
    device.loop();
}
