
#include <vector>
#include <map>
#include "Arduino.h"
#include "PubSubClient.h"
#include "WiFiManager.h"
#include "ticker.h"
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

    std::vector<Sensor> sensors;
    std::vector<Input*> inputs;
    PubSubClient mqtt;

    void initSerial();
    void initConfig();
    void saveConfig();
    void initWiFi();
    void reconnectWiFi();
    void initMQTT();
    void reconnectMQTT();

  public:
    // myiot_config config;
    std::map<String, myiot_config*> config;

    Device(String model);
    // Device(String model, myiot_config default_config);
    ~Device();

    void setup();
    void loop();

    void addConfig(String name, size_t size, String default_value);
    char* getConfig(String name);

    void addSensor(Sensor &s);
    Sensor &addSensor(String name, uint8 pin);
    Sensor &addSensor(String name, std::function<String()>);

    Input& addInput(const char* name, uint8_t pin, uint8_t pin_mode = INPUT);

    void readSensors();

}; // Device






class Button : public Input
{
  private:
    /* data */
    // String name;
    // uint8_t pin;
    // uint8_t input_mode;
    // bool is_pullup;
    // // bool is_internal_pullup;
    // std::map<uint8_t, std::function<void()>> click_callbacks;
    // bool is_down;
    // unsigned long last_down;
    // unsigned long last_up;

    // uint8_t pending_clicks;
  public:
    Button(const char* name, uint8_t pin, uint8_t pin_mode = INPUT) : Input(name, pin, pin_mode)
    {
        is_button = true;
    }
    // void setup();
    // void onClicks(uint8_t number_of_clicks, std::function<void()> callback);
    // void loop();
}; // Button

} // namespace myiot


