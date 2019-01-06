#include <Arduino.h>
#include "myiot.h"
// #include <Adafruit_Sensor.h>
// #include "DHT.h"

using namespace myiot;

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

    // setup
    device.setup();

    // cmd
    device.addCommand("report", [](byte* payload){
        Serial.println("WOOOOOOHOOOOOOOO");
    });

    device.addTicker(1000, []() {
        device.runCommand("report");
    });

}

void loop()
{
    device.loop();
}
