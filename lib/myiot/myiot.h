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
  private:
    String model;

    WiFiManager wifi_manager;
    PubSubClient mqtt;

    std::vector<Input*> inputs;
    std::vector<Ticker*> tickers;

    void initConfig();
    void saveConfig();
    void initWiFi();
    void initMQTT();
    void reconnectWiFi();
    void reconnectMQTT();
  public:

    std::map<String, myiot_config*> config;

    Device(String model);

    void setup();
    void loop();

    void addConfig(String name, size_t size, String default_value);
    char* getConfig(String name);

    Input* addInput(const char* name, uint8_t pin);
    Ticker* addTicker(unsigned long interval, std::function<void()> callback);

    void publishInput(Input*, bool retained = false);


}; // Device





} // namespace myiot


