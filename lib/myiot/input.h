#pragma once
#include <Arduino.h>

namespace myiot
{

class Input
{
protected:
  uint8_t pin;
  uint8_t pin_mode;
  const char* name;

  unsigned long last_read;
  unsigned long last_publish;

  bool is_enabled;
  bool is_sensor;
  bool is_button;

  // template accepts int or float
  template<typename T> void _loop(T* value, std::function<T()> reader);

public:
  int value;
  float float_value;
  bool is_analog;
  uint16_t debounce_ms;

  std::function<void(Input&)> on_change;
  std::function<int()> read_int;
  std::function<float()> read_float;
  std::function<String()> to_string;

  Input(const char* name, uint8_t pin, uint8_t pin_mode);
  // ~Input();

  // const char* name()
  // {
  //   return name;
  // }

  virtual void setup();
  virtual void loop();

  const char* getName() {
    return name;
  };

  void setEnabled(bool v) {
    is_enabled = v;
  }


};


}