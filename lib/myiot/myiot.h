#pragma once
#include <vector>
#include <map>
#include "Arduino.h"
#include "PubSubClient.h"
#include "WiFiManager.h"
#include "ticker.h"
#include "button.h"
#include "input.h"
#include "sensor.h"


struct myiot_config
{
    String default_value;
    size_t size;
    char* value;
    WiFiManagerParameter* wifi_parameter;
};


namespace myiot
{


class Device
{
  protected:
    String model;

    WiFiManager wifi_manager;
    PubSubClient mqtt;

    std::vector<Input*> inputs;
    std::vector<Ticker*> tickers;
    std::map<String, std::function<void(byte*)>> commands;
    std::map<String, myiot_config*> config;

    void initConfig();
    void saveConfig();
    void initWiFi();
    void initMQTT();
    void reconnectWiFi();
    void reconnectMQTT();

  public:

    Device(String model);

    void setup();
    void loop();

    void addConfig(String name, size_t size, String default_value);
    char* getConfig(String name);

    Ticker* addTicker(unsigned long interval, std::function<void()> callback);

    Input* addInput(const String &name, uint8_t pin);
    Input* input(const String &name);
    void publishInput(Input*, bool retained = false);

    bool hasCommand(const String &name);
    void addCommand(const String &name, std::function<void(byte*)> callback);
    void runCommand(const String &name, byte* payload = NULL);


}; // Device





} // namespace myiot


