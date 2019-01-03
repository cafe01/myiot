#include <Arduino.h>
#include "myiot.h"
// #include <Adafruit_Sensor.h>
// #include "DHT.h"

using namespace myiot;

myiot::Device device("myiot");

// myiot::Input digital_input("test_digital_input", D2);
// myiot::Input analog_input("test_analog_input", A0);


// myiot::Button btn("flashBtn", 0);

void foo();


Ticker t1(1000, foo);

void setup()
{
    // custom config
    device.addConfig("custom_cfg", 40, "waravalue");

    // inputs
    // auto digital = device.addInput("test_digital", D2);

    // setup
    device.setup();
    t1.start();



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
    t1.loop();

    if (t1.getCount() == 5)
    {
        t1.setInterval(250);
    }

    if (t1.getCount() == 15) t1.stop();

}


void foo()
{
    Serial.printf("%d Done foo!\n", (int)t1.getCount());
}