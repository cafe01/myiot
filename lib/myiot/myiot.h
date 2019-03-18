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
#include "led.h"


namespace myiot
{

struct myiot_config
{
    const char* name;
    size_t size;
    const char* default_value;
    char* value;
    WiFiManagerParameter* wifi_parameter;
};

typedef std::function<void(const char* payload)> mqtt_subscription_callback;
typedef std::function<void(Input*)> input_callback;
typedef std::function<void(Output*)> output_callback;
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
    uint8_t status_led;
    WiFiManager wifi_manager;
    PubSubClient mqtt;

    std::vector<Output*> outputs;
    std::vector<Input*> inputs;
    std::vector<Ticker*> tickers;
    std::map<String, mqtt_subscription_callback> commands;
    std::vector<mqtt_subscription*> subscriptions;
    std::vector<myiot_config*> config;

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

    void restart();
    void startConfigPortal();

    void setStatusLED(uint8_t pin)
    {
        pinMode(pin, OUTPUT);
        status_led = pin;
    };

    Ticker* addTicker(unsigned long interval, std::function<void()> callback);

    // config
    void addConfig(const char* name, size_t size, const char* default_value);
    char* getConfig(const char* name);
    void setConfig(const char*, const char* value);
    void resetConfig();

    // TODO
    // void add(Input*);
    // void add(Output*);

    // input
    void addInput(Input*);
    Input* addInput(const String &name, uint8_t pin);
    Input* input(const String &name);

    // output
    void addOutput(Output*);
    Output* addOutput(const char* name, uint8_t pin);
    Output* output(const char* name);

    // each
    void eachInput(input_callback);
    void eachOutput(output_callback);

    // subscribe
    void subscribe(const char* topic, mqtt_subscription_callback callback);
    bool hasSubscription(const char* topic);

    // publish
    bool publish(const char* topic, const char* payload);
    bool publish(Input*, bool retained = false);

    // commands
    void addCommand(const String &name, mqtt_subscription_callback callback);
    void runCommand(const String &name, const char* payload = NULL);
    bool hasCommand(const String &name);

    // telemetry
    void sendTelemetry();


}; // Device





} // namespace myiot


