#pragma once
#include <Arduino.h>

namespace myiot
{

class Input
{
protected:
  String name;
  uint8_t pin;

  unsigned long last_read = 0;
  unsigned long last_publish = 0;

  bool is_enabled = true;
  bool is_sensor = false;
  bool is_button = false;

  std::vector<std::function<void(Input*)>> on_change;

  // template accepts int or float
  template<typename T> void _loop(T* value, std::function<T()> reader);

public:
  int value = 0;
  float float_value = 0;
  bool is_analog = false;
  bool publish_on_change;
  unsigned long last_change;
  unsigned long read_interval = 0;
  uint16_t debounce_ms = 10;

  std::function<int()> read_int;
  std::function<float()> read_float;
  std::function<String()> to_string;

  Input(const String &name);
  Input(const String &name, uint8_t pin);
  // ~Input();

  // const char* name()
  // {
  //   return name;
  // }

  virtual void setup();
  virtual void loop();

  String getName() {
    return name;
  };

  void setEnabled(bool v) {
    is_enabled = v;
  }

  unsigned int onChange(std::function<void(Input*)> cb)
  {
    on_change.push_back(cb);
    return on_change.size();
  }




};


}