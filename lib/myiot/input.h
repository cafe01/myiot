#pragma once
#include <Arduino.h>

namespace myiot
{

class Input
{
protected:
  String name;
  uint8_t pin;

  unsigned long last_read;
  unsigned long last_publish;

  bool is_enabled;
  bool is_sensor;
  bool is_button;

  std::vector<std::function<void(Input*)>> on_change;

  // template accepts int or float
  template<typename T> void _loop(T* value, std::function<T()> reader);

public:
  int value;
  float float_value;
  bool is_analog;
  bool publish_on_change;
  unsigned long read_interval;
  uint16_t debounce_ms;

  std::function<int()> read_int;
  std::function<float()> read_float;
  std::function<String()> to_string;

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