#include <Arduino.h>
#include "myiot.h"
// #include <Adafruit_Sensor.h>
// #include "DHT.h"

using namespace myiot;

myiot::Device device("myiot");

// myiot::Input digital_input("test_digital_input", D2);
// myiot::Input analog_input("test_analog_input", A0);

void setup()
{
    // custom config
    // device.addConfig("custom_cfg", 40, "waravalue");

    // inputs
    auto digital = device.addInput("icos01", D2);
    digital->debounce_ms = 3;

    auto dev = &device;

    digital->onChange([dev](Input *input) -> void {
        Serial.printf("[cb] input %s changed to %d\n", input->getName(), input->value);
        // dev->publishInput(&input, true);
    });

    // setup
    device.setup();
}

void loop()
{
    device.loop();
}
