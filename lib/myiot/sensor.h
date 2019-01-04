#pragma once
#include <Arduino.h>
#include "PubSubClient.h"

namespace myiot
{

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
    };

}