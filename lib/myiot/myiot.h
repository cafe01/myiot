#pragma once
#include <vector>
#include <map>
#include "Arduino.h"
#include "PubSubClient.h"
#include "WiFiManager.h"
#include "ticker.h"
#include "button.h"
#include "input.h"
#include "output.h"


namespace myiot
{

struct myiot_config
{
    String default_value;
    size_t size;
    char* value;
    WiFiManagerParameter* wifi_parameter;
};

typedef std::function<void(const char* payload)> mqtt_subscription_callback;
struct mqtt_subscription
{
    const char* topic;
    bool is_subscribed;
    mqtt_subscription_callback callback;
};

class Device
{
  protected:
    String model;

    WiFiManager wifi_manager;
    PubSubClient mqtt;

    std::vector<Output*> outputs;
    std::vector<Input*> inputs;
    std::vector<Ticker*> tickers;
    std::map<String, mqtt_subscription_callback> commands;
    std::vector<mqtt_subscription*> subscriptions;
    std::map<String, myiot_config*> config;

    void initConfig();
    void saveConfig();
    void initWiFi();
    void initMQTT();
    void reconnectWiFi();
    void reconnectMQTT();

    void mqttCallback(char* topic, byte* payload, unsigned int size);
  public:

    Device(String model);

    void setup();
    void loop();

    void addConfig(String name, size_t size, String default_value);
    char* getConfig(const String &name);

    Ticker* addTicker(unsigned long interval, std::function<void()> callback);

    void addInput(Input*);
    Input* addInput(const String &name, uint8_t pin);
    Input* input(const String &name);
    void publishInput(Input*, bool retained = false);

    Output* addOutput(const char* name, uint8_t pin);
    Output* output(const char* name);

    void publish(const char* topic, const String &payload);
    void subscribe(const char* topic, mqtt_subscription_callback callback);
    bool hasSubscription(const char* topic);

    void addCommand(const String &name, mqtt_subscription_callback callback);
    void runCommand(const String &name, const char* payload = NULL);
    bool hasCommand(const String &name);


}; // Device





} // namespace myiot


