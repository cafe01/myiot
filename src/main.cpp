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
    device.addConfig("custom_cfg", 40, "waravalue");

    // inputs
    auto digital = device.addInput("test_digital", 0);
    digital->on_change = [](Input& input) -> void {
        Serial.printf("[cb] input %s changed to %d\n", input.getName(), input.value);
    };




    // setup
    device.setup();



    // btn.onClicks(1, []() -> void {
    //     Serial.println("Click handler");
    // });

    // btn.onClicks(2, [](){ Serial.println("Clicked 2"); });
    // btn.onClicks(3, [](){ Serial.println("Clicked 3"); });


    // ESP.reset();
    // wifiManager.resetSettings();
    // wifiManager.autoConnect("wara");
}

void loop()
{
    device.loop();
}

