#include <Arduino.h>
#include "myiot.h"
#include <Adafruit_Sensor.h>
#include "DHT.h"

#define MODEL "ipcc"
#define LDRPIN A0
#define DHTPIN D4


myiot::Device device(MODEL);


void setup()
{

    // default config
    device.config.wifi_ssid = "VIVO-26E0";
    device.config.wifi_password = "32359498";
    device.config.mqtt_host = "openhabianpi.local";
    device.config.mqtt_port = "1883";
    device.config.loop_delay = 100;
    device.config.name = "ipcc";
    // device.config.disable_serial = true;


    // temperature sensor
    DHT *dht = new DHT(DHTPIN, DHT22);

    device.addSensor("temperature", [dht]() -> String {
        return String(dht->readTemperature());
    });

    // humidity sensor
    device.addSensor("humidity", [dht]() -> String {
        return String(dht->readHumidity());
    });

    // light sensor
    auto light_sensor = device.addSensor("light", [dht]() -> String {
        auto val = analogRead(LDRPIN);
        return String(val);
    });

    light_sensor.pin = LDRPIN;
    light_sensor.is_analog = true;

    device.setup();


}

void loop()
{
    device.loop();
}
