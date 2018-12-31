
#include <vector>
#include <map>
#include "Arduino.h"
#include "PubSubClient.h"
#include "WiFiManager.h"

struct myiot_config
{
    String default_value;
    size_t size;
    char* value;
    WiFiManagerParameter* wifi_parameter;
};

namespace myiot
{


class Sensor;

class Device
{
  private:
    String model;
    WiFiManager wifi_manager;

    std::vector<Sensor> sensors;
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
    void readSensors();

}; // Device




class Sensor
{
  private:
    int last_read;
    String current_value;

  public:
    String name;
    uint8 pin;
    bool is_analog;
    bool is_pullup;
    unsigned int read_interval;
    std::function<String()> reader;

    Sensor(String name, uint8 pin);
    Sensor(String name, std::function<String()> reader);
    ~Sensor();

    void setup();
    bool canRead();
    String value();
    String read();
    void mqtt_publish(PubSubClient &mqtt);
}; // Sensor

class Button
{
  private:
    /* data */
    String name;
    uint8_t pin;
    uint8_t input_mode;
    bool is_pullup;
    // bool is_internal_pullup;
    std::map<uint8_t, std::function<void()>> click_callbacks;
    bool is_down;
    unsigned long last_down;
    unsigned long last_up;

    uint8_t pending_clicks;
  public:
    Button(String name, uint8_t pin);
    Button(String name, uint8_t pin, uint8_t mode);
    void setup();
    void onClicks(uint8_t number_of_clicks, std::function<void()> callback);
    void loop();
}; // Button

} // namespace myiot


