
#include "myiot.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
// #include <PubSubClient.h>
#include "FS.h"

namespace myiot
{

// constructor
Device::Device(String model)
{
    this->model = model;
    this->config = {};

    WiFiClient espClient;
    mqtt = *(new PubSubClient(espClient));
}

// destructor
Device::~Device() {}

// addSensor()
Sensor &Device::addSensor(String name, uint8 pin)
{
    sensors.resize(sensors.size() + 1, {name, pin});
    return sensors.back();
}
Sensor &Device::addSensor(String name, std::function<String()> reader)
{
    sensors.resize(sensors.size() + 1, {name, reader});
    return sensors.back();
}
void Device::addSensor(Sensor &s)
{
    sensors.reserve(sensors.size() + 1);
    sensors.push_back(s);
}

// setup()
void Device::setup()
{
    // setup serial
    if (!config.disable_serial)
        initSerial();

    // load config
    initConfig();

    // wifi
    initWiFi();

    // mqtt


    // TODO web server

    // setup sensors
    Serial.printf("Sensors: %d\n", (int)sensors.size());
    for (std::size_t i = 0; i < sensors.size(); i++)
        sensors[i].setup();

    // sensible defaults
    if (config.loop_delay == 0)
        config.loop_delay = 1000;
}

void Device::initSerial()
{
    delay(500);
    Serial.begin(9600);
    Serial.println("\nMyIOT started!");
}

void load_config(const char *file_name, String &target)
{

    // not exists
    if (!SPIFFS.exists(file_name))
    {
        Serial.printf("[Config] undefined: %s\n", file_name);
        return;
    }

    File file = SPIFFS.open(file_name, "r");
    if (!file)
    {
        Serial.printf("[ERROR] Could not open file '%s'\n", file_name);
        return;
    }

    target = file.readString();
    Serial.printf("[Config] loaded %s (%s)\n", file_name, target.c_str());
}

void Device::initConfig()
{

    SPIFFS.begin();
    load_config("/wifi.ssid", config.wifi_ssid);
    load_config("/wifi.password", config.wifi_password);
    load_config("/mqtt.host", config.mqtt_host);
    load_config("/mqtt.port", config.mqtt_port);
    load_config("/Device.name", config.name);

    // set defaults

    // name based on mac address
    if (config.name == "")
    {
        String suffix = WiFi.macAddress().substring(12);
        suffix.replace(":", "");
        suffix.toLowerCase();
        config.name = this->model + "-" + suffix;
    }

    // loop_delay

    Serial.println("==== Current config ====");
    Serial.printf("> name: %s\n", config.name.c_str());
    Serial.printf("> wifi_ssid: %s\n", config.wifi_ssid.c_str());
    Serial.printf("> wifi_password: %s\n", config.wifi_password.c_str());
    Serial.printf("> mqtt_host: %s\n", config.mqtt_host.c_str());
    Serial.printf("> mqtt_port: %s\n", config.mqtt_port.c_str());
    Serial.printf("> loop_delay: %d\n", config.loop_delay);
}

void Device::initWiFi()
{

    // init
    delay(500);
    Serial.println("[WiFi] Initializing...");
    Serial.println("[WiFi] SSID: " + config.wifi_ssid);
    Serial.println("[WiFi] MAC: " + WiFi.macAddress());

    // mDNS
    if (MDNS.begin(config.name.c_str()))
        Serial.println("[WiFi] mDNS host: " + config.name + ".local");
    else
        Serial.println("[WiFi] Error setting up mDNS responder!");
}


void Device::reconnectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
        return;

    // TODO check for empty wifi config

    Serial.printf("[WiFi] Connecting to '%s' ", config.wifi_ssid.c_str());
    WiFi.begin(config.wifi_ssid.c_str(), config.wifi_password.c_str());

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        Serial.print(".");
        // TODO implement timeout
    }

    Serial.println(" connected!");
    Serial.println("[WiFi] IP: " + WiFi.localIP().toString());
}

void Device::reconnectMQTT()
{
    if (this->mqtt.connected()) return;

    delay(100);

    this->mqtt.setServer(config.mqtt_host.c_str(), 1883);
    Serial.printf("[MQTT] Connecting to '%s' ... ", config.mqtt_host.c_str());

    if (this->mqtt.connect(config.name.c_str()))
    {

        Serial.printf("connected\n");
        Serial.printf("[MQTT] client id: %s\n", config.name.c_str());
        // MQTT.subscribe(cmd_topic.c_str());
    }
    else
    {
        Serial.printf("error: %d\n", this->mqtt.state());
    }
}
} // namespace myiot

void myiot::Device::readSensors()
{
    // Serial.printf("Reading %d sensors:\n", sensors.size());

    for (std::size_t i = 0; i < sensors.size(); i++)
    {

        Sensor &s = sensors[i];

        if (!s.canRead()) continue;

        // read
        auto value = s.read();
        Serial.printf("[Sensor] %s: %s\n", s.name.c_str(), value.c_str());

        // publish
        if (mqtt.connected())
        {
            // raw
            String topic = "stat/" + config.name + "/" + s.name;
            mqtt.publish(topic.c_str(), value.c_str());

            // json
            if (config.mqtt_publish_json)
            {
                String json_payload = "{ \"value\": " + value + " }";
                mqtt.publish((topic + "/json").c_str(), json_payload.c_str());
            }
        }
    }
}

void myiot::Device::loop()
{
    // loop delay
    if (millis() - last_loop < config.loop_delay)
        return;

    last_loop = millis();

    // loop routines
    reconnectWiFi();
    reconnectMQTT();
    readSensors();
}
