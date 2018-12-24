
#include <vector>
#include "Arduino.h"
#include "PubSubClient.h"



struct myiot_config
{
    String name;
    String wifi_ssid;
    String wifi_password;
    uint wifi_connect_timeout;

    String mqtt_host;
    String mqtt_port;

    uint loop_delay;

    bool disable_serial;
    bool mqtt_publish_json;
};


namespace myiot
{
    class Sensor;

    class Device
    {
    private:
        String model;
        ulong last_loop;
        std::vector<Sensor> sensors;
        PubSubClient mqtt;

        void initSerial();
        void initConfig();
        void initWiFi();
        void reconnectWiFi();
        void initMQTT();
        void reconnectMQTT();

    public:
        myiot_config config;

        Device(String model);
        // Device(String model, myiot_config default_config);
        ~Device();

        void setup();
        void loop();
        void addSensor(Sensor &s);
        Sensor& addSensor(String name, uint8 pin);
        Sensor& addSensor(String name, std::function<String()>);
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
        void mqtt_publish(PubSubClient& mqtt);
    };



} // myiot


