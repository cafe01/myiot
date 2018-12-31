#include <Arduino.h>
#include "myiot.h"
// #include <Adafruit_Sensor.h>
// #include "DHT.h"


myiot::Device device("myiot");

myiot::Button btn("flashBtn", 0);

void setup()
{
    device.addConfig("custom_cfg", 40, "waravalue");

    device.setup();
    btn.setup();


    btn.onClicks(1, []() -> void {
        Serial.println("Click handler");
    });

    // btn.onClicks(2, [](){ Serial.println("Clicked 2"); });
    // btn.onClicks(3, [](){ Serial.println("Clicked 3"); });


    // ESP.reset();
    // wifiManager.resetSettings();
    // wifiManager.autoConnect("wara");
}

void loop()
{
    device.loop();
    btn.loop();

}
