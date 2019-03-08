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

void Device::addOutput(Output* output)
{
    outputs.push_back(output);
}

Output* Device::addOutput(const char* name, uint8_t pin)
{
    auto output = new Output(name, pin);
    outputs.push_back(output);
    return output;
}

Output* Device::output(const char* name)
{
    for (size_t i = 0; i < outputs.size(); i++)
        if (strcmp(outputs[i]->name, name) == 0)
            return outputs[i];

    return NULL;
}

// addInput()
void Device::addInput(Input* input)
{
    inputs.push_back(input);
}

Input* Device::addInput(const String &name, uint8_t pin)
{
    auto input = new Input(name, pin);
    inputs.push_back(input);
    return input;
}

// input()
Input* Device::input(const String &name)
{
    Input* input = NULL;

    for (size_t i = 0; i < inputs.size(); i++)
        if (inputs[i]->getName() == name)
        {
            input = inputs[i];
            break;
        }

    if (input == NULL)
        Serial.printf("[ERROR] unknown input '%s'\n", name.c_str());

    return input;
}

// addConfig()
void Device::addConfig(const char* name, size_t size, const char* default_value)
{
    myiot_config* cfg = new myiot_config();

    cfg->name = name;
    cfg->size = size;
    cfg->default_value = default_value;

    cfg->value = new char[size];
    strcpy(cfg->value, default_value);
    cfg->wifi_parameter = NULL;

    config.push_back(cfg);
}

// getConfig()
char* Device::getConfig(const char* name)
{
    for (size_t i = 0; i < config.size(); i++)
        if (strcmp(config[i]->name, name) == 0)
            return config[i]->value;

    Serial.printf("[ERROR] getConfig: unknown config '%s'\n", name);
    return 0;
}

void Device::setConfig(const char* name, const char* value)
{
    for (size_t i = 0; i < config.size(); i++)
        if (strcmp(config[i]->name, name) == 0)
            strncpy(config[i]->value, value, config[i]->size);

    MYIOT_SAVE_CONFIG = true;
}

Ticker* Device::addTicker(unsigned long interval, std::function<void()> callback)
{
    auto ticker = new Ticker(interval);
    ticker->callback = callback;
    tickers.push_back(ticker);
    return ticker;
}

bool Device::hasCommand(const String &name)
{
    return commands.find(name) != commands.end();
}

void Device::addCommand(const String &name, mqtt_subscription_callback callback)
{
    // soft-exception
    if (hasCommand(name))
    {
        Serial.printf("[WARNING] addCommand(): cmd '%s' already exists. You should take some rest..\n", name.c_str());
        return;
    }

    // save callback
    commands[name] = callback;

    // subscribe to command topic
    String topic = "cmnd/" + String(getConfig("name")) + "/" + name;
    subscribe(topic.c_str(), [callback](const char* payload) {
        // Serial.printf("ON subscribe handler!!! (%s)\n", payload);
        callback(payload);
    });
}

void Device::runCommand(const String &name, const char* payload)
{
    // soft-exception
    if (!hasCommand(name))
    {
        Serial.printf("[WARNING] runCommand(): unknown cmd '%s'. You should take some rest..\n", name.c_str());
        return;
    }
    Serial.printf("[runCommand] %s\n", name.c_str());
    commands[name](payload);
}


void Device::subscribe(const char* topic, mqtt_subscription_callback callback)
{
    if (hasSubscription(topic))
    {
        Serial.printf("[!!] already subscribed to topic '%s'", topic);
        return;
    }

    auto new_subscription = new mqtt_subscription {
        (new String(topic))->c_str(),
        false,
        callback
    };

    subscriptions.push_back(new_subscription);
}


bool Device::hasSubscription(const char* topic)
{
    for (size_t i = 0; i < subscriptions.size(); i++)
    {
        if (strcmp(subscriptions[i]->topic, topic) == 0)
            return true;
    }
    return false;
}





// setup()
void Device::setup()
{
    // setup serial
    delay(500);
    Serial.begin(9600);
    delay(500);
    Serial.println("\nMyIOT started!");

    // dev only
    // wifi_manager.resetSettings();

    // load config
    initConfig();

    // wifi
    initWiFi();

    // mqtt
    mqtt.setCallback([this](char* topic, byte* payload, unsigned int size) -> void {
        mqttCallback(topic, payload, size);
    });

    // inputs
    for (size_t i = 0; i < inputs.size(); i++)
        inputs[i]->setup();

    // outputs
    for (size_t i = 0; i < outputs.size(); i++)
        outputs[i]->setup();

    // loop tasks
    addTicker(1000, [this]() -> void { this->reconnectWiFi(); });
    addTicker(1000, [this]() -> void { this->reconnectMQTT(); });
    addTicker(100, [this]() -> void { this->saveConfig(); });

    // TODO add telemetry

    // reconnectWiFi();
}

void Device::initConfig()
{

    // mount fs
    if (!SPIFFS.begin())
    {
        Serial.println("[ERROR] error mounting file system.");
        return;
    }

    // reset config
    if (resetConfig && SPIFFS.exists("/config.json"))
    {
        if (SPIFFS.remove("/config.json"))
            Serial.println("!!! CONFIG FILE DELETED! (remember to disable) ");
        else
            Serial.println("[ERROR] could not delete config.json");
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

                for (size_t i = 0; i < config.size(); i++)
                {
                    auto cfg = config[i];
                    if (json.containsKey(cfg->name))
                        strcpy(cfg->value, json[cfg->name]);
                };

            } else {
                Serial.println("failed to load json config");
            }
            configFile.close();
        }
    }

    // create wifi parameters
    // NOTE new config values wont be reflected on webadmin
    for (size_t i = 0; i < config.size(); i++)
        config[i]->wifi_parameter = new WiFiManagerParameter(config[i]->name, config[i]->default_value, config[i]->value, config[i]->size);


    Serial.println("==== Current config ====");
    for (size_t i = 0; i < config.size(); i++)
    {
        Serial.printf("> %s: %s\n", config[i]->name, config[i]->value);
    };
}

void Device::saveConfig()
{
    if (!MYIOT_SAVE_CONFIG) return;

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();

    for (size_t i = 0; i < config.size(); i++)
        json[config[i]->name] = config[i]->value;

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
    wifi_manager.setTimeout(120);

    // add config parameters
    for (size_t i = 0; i < config.size(); i++)
        wifi_manager.addParameter(config[i]->wifi_parameter);

    wifi_manager.setSaveConfigCallback([]() -> void {
        MYIOT_SAVE_CONFIG = true;
    });
}

void Device::reconnectWiFi()
{
    if (WiFi.status() == WL_CONNECTED)
        return;

    if (status_led)
        digitalWrite(status_led, HIGH);

    if(!wifi_manager.autoConnect(getConfig("name")))
    {
        Serial.println("[WiFi] managet autoConnect() timedout.");
        Serial.println("Restarting device...");
        delay(3000);
        ESP.reset();
        delay(6000);
    }
    else
    {
        Serial.println("[WiFi] Connected! IP: " + WiFi.localIP().toString());

        // load config values from wifiparams
        for (size_t i = 0; i < config.size(); i++)
            strcpy(config[i]->value, config[i]->wifi_parameter->getValue());
    }

    if (status_led)
        digitalWrite(status_led, LOW);
}

void Device::reconnectMQTT()
{
    // already connected, send pending subscriptions
    if (mqtt.connected())
    {
        for (size_t i = 0; i < subscriptions.size(); i++)
        {
            auto item = subscriptions[i];

            // skip already subscribed
            if (item->is_subscribed) continue;

            // subscribe
            if (mqtt.subscribe(item->topic))
            {
                Serial.printf("[MQTT] subscribed to '%s'\n", item->topic);
                item->is_subscribed = true;
            }
            else
                Serial.printf("[!!] [MQTT] couldn't subscribe to '%s'\n", item->topic);
        }

        return;
    }

    // (re)connect!
    auto host = getConfig("mqtt_host");
    mqtt.setServer(host, 1883);
    Serial.printf("[MQTT] Connecting to '%s' ... ", host);

    auto client_id = getConfig("name");
    if (mqtt.connect(client_id))
        Serial.printf("connected. (client_id: %s)\n", client_id);
    else
        Serial.printf("error: %d\n", mqtt.state());
}

void Device::mqttCallback(char* topic, byte* payload, unsigned int size)
{
    // copy payload
    // TODO use stack variable
    // char[MQTT_MAX_PACKET_SIZE] content_buf;
    char *content = new char[size+1];
    strncpy(content, (const char*)payload, size);
    content[size] = '\0';

    Serial.printf("[mqttCallback] '%s': (%d bytes) '%s'\n", topic, size, content);

    // run callbacks
    for (size_t i = 0; i < subscriptions.size(); i++)
    {
        if (strcmp(topic, subscriptions[i]->topic) == 0)
            subscriptions[i]->callback(content);
    }

    // delete content
    free(content);
}

void Device::publish(const char* topic, const char* payload)
{
    mqtt.publish(topic, payload);
}

void Device::publishInput(Input* input, bool retained)
{
    if (!mqtt.connected())
    {
        Serial.println("[MQTT] publishInput skipped (mqtt is disconnected)");
        return;
    }
    String topic = "stat/" + String(getConfig("name")) + "/" + input->getName();
    String payload = input->to_string();
    Serial.printf("[MQTT] publishing to '%s': '%s'\n", topic.c_str(), payload.c_str());
    mqtt.publish(topic.c_str(), payload.c_str(), retained);
}


// loop
void Device::loop()
{
    // tickers
    for (size_t i = 0; i < tickers.size(); i++) {
        tickers[i]->loop();
    }

    // inputs
    for (size_t i = 0; i < inputs.size(); i++) {
        inputs[i]->loop();
    }

    //
    mqtt.loop();
}

}


