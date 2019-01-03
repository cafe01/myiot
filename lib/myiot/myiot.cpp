
#include "myiot.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
// #include <PubSubClient.h>
#include "FS.h"
#include "WiFiManager.h"
#include "ArduinoJson.h"


namespace myiot
{
bool MYIOT_SAVE_CONFIG = false;




// constructor
Device::Device(String model)
{
    this->model = model;


    WiFiClient *espClient = new WiFiClient();
    mqtt = *(new PubSubClient(*espClient)); // TODO make mqtt a pointer

    String default_name = model + "-" + WiFi.macAddress().substring(12);
    default_name.replace(":", "");
    default_name.toLowerCase();

    addConfig("name", 32, default_name.c_str());
    addConfig("mqtt_host", 40, "openhabianpi.local");
    addConfig("mqtt_port", 6, "1883");

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

void Device::addConfig(String name, size_t size, String default_value)
{
    myiot_config* cfg = new myiot_config();

    cfg->size = size;
    cfg->default_value = default_value;
    cfg->value = new char[size];
    strcpy(cfg->value, default_value.c_str());
    cfg->wifi_parameter = NULL;

    config[name] = cfg;
}

char* Device::getConfig(String name)
{
    auto it = config.find(name);
    if (it == config.end())
        Serial.printf("[ERROR] invalid config '%s'\n", name.c_str());

    return it->second->value;
}

// setup()
void Device::setup()
{
    // setup serial
    initSerial();

    // dev only
    // wifi_manager.resetSettings();

    // load config
    initConfig();

    // wifi
    initWiFi();


    // TODO web server

    // setup sensors
    // Serial.printf("Sensors: %d\n", (int)sensors.size());
    // for (std::size_t i = 0; i < sensors.size(); i++)
    //     sensors[i].setup();
}

void Device::initSerial()
{
    delay(1000);
    Serial.begin(9600);
    Serial.println("\nMyIOT started!");
}

void Device::initConfig()
{

    // mount fs
    if (!SPIFFS.begin())
    {
        Serial.println("[ERROR] error mounting file system.");
        return;
    }

    // load existing config
    if (SPIFFS.exists("/config.json"))
    {
        Serial.println("[Config] reading config file");
        File configFile = SPIFFS.open("/config.json", "r");
        if (configFile) {
            Serial.println("opened config file");
            size_t size = configFile.size();
            std::unique_ptr<char[]> buf(new char[size]);
            configFile.readBytes(buf.get(), size);
            DynamicJsonBuffer jsonBuffer;
            JsonObject& json = jsonBuffer.parseObject(buf.get());
            json.printTo(Serial);
            if (json.success()) {
                Serial.println("\nparsed json");
                for (auto it = config.begin(); it != config.end(); ++it)
                {
                    auto cfg = it->second;
                    if (json.containsKey(it->first))
                        strcpy(cfg->value, json[it->first]);
                };

            } else {
                Serial.println("failed to load json config");
            }
            configFile.close();
        }
    }

    // create wifi parameters
    // NOTE new config values wont be reflected on webadmin
    for (auto it = config.begin(); it != config.end(); ++it)
    {
        auto cfg = it->second;
        cfg->wifi_parameter = new WiFiManagerParameter(it->first.c_str(), cfg->default_value.c_str(), cfg->value, cfg->size);
    };


    Serial.println("==== Current config ====");
    for (auto it = config.begin(); it != config.end(); ++it)
    {
        Serial.printf("> %s: %s\n", it->first.c_str(), it->second->value);
    };
}

void Device::saveConfig()
{

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();

    for (auto it = config.begin(); it != config.end(); it++)
    {
        auto cfg = it->second;
        json[it->first] = cfg->value;
    }

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
        Serial.println("failed to open config file for writing");
    }

    Serial.println("[Config] saving config: ");
    json.printTo(Serial);
    Serial.println("");

    json.printTo(configFile);
    configFile.close();

    MYIOT_SAVE_CONFIG = false;
}

void Device::initWiFi()
{

    // init
    Serial.println("[WiFi] Initializing...");
    Serial.println("[WiFi] SSID: " + WiFi.SSID());
    Serial.println("[WiFi] MAC: " + WiFi.macAddress());

    // mDNS
    if (MDNS.begin(getConfig("name")))
        Serial.printf("[WiFi] mDNS host: %s.local\n", getConfig("name"));
    else
        Serial.println("[WiFi] Error setting up mDNS responder!");


    // portal timeout
    wifi_manager.setTimeout(180);

    // add config parameters
    for (auto it = config.begin(); it != config.end(); ++it)
    {
        auto cfg = it->second;
        wifi_manager.addParameter(cfg->wifi_parameter);
    };

    wifi_manager.setSaveConfigCallback([]() -> void {
        MYIOT_SAVE_CONFIG = true;
    });
}

void Device::reconnectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
        return;

    if(!wifi_manager.autoConnect(getConfig("name"))) {
        Serial.println("[WiFi] managet autoConnect() timedout.");
        delay(3000);
    }
    else
    {
        Serial.println("[WiFi] Connected! IP: " + WiFi.localIP().toString());
        for (auto it = config.begin(); it != config.end(); it++)
        {
            auto cfg = it->second;
            strcpy(cfg->value, cfg->wifi_parameter->getValue());
        }
    }
}

void Device::reconnectMQTT()
{
    if (mqtt.connected()) return;


    // this->mqtt.setServer(config.mqtt_host.c_str(), 1883);
    // Serial.printf("[MQTT] Connecting to '%s' ... ", config.mqtt_host.c_str());

    // if (this->mqtt.connect(config.name.c_str()))
    // {

    //     Serial.printf("connected\n");
    //     Serial.printf("[MQTT] client id: %s\n", config.name.c_str());
    //     // MQTT.subscribe(cmd_topic.c_str());
    // }
    // else
    // {
    //     Serial.printf("error: %d\n", this->mqtt.state());
    // }
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
        // auto value = s.read();
        // Serial.printf("[Sensor] %s: %s\n", s.name.c_str(), value.c_str());

        // // publish
        // if (mqtt.connected())
        // {
        //     // raw
        //     String topic = "stat/" + config.name + "/" + s.name;
        //     mqtt.publish(topic.c_str(), value.c_str());

        //     // json
        //     if (config.mqtt_publish_json)
        //     {
        //         String json_payload = "{ \"value\": " + value + " }";
        //         mqtt.publish((topic + "/json").c_str(), json_payload.c_str());
        //     }
        // }
    }
}

void myiot::Device::loop()
{
    if (MYIOT_SAVE_CONFIG) saveConfig();

    reconnectWiFi();
    // reconnectMQTT();
    // readSensors();

    // inputs
    for (size_t i = 0; i < inputs.size(); i++)
    {
        inputs[i]->loop();
    }
}
