#include <Arduino.h>

namespace myiot
{

class Input
{
protected:
  uint8_t pin;
  uint8_t pin_mode;
  const char* name;
  int value;

  unsigned long last_read;
  unsigned long last_publish;

  bool is_sensor;
  bool is_button;

public:
  bool is_analog;
  uint16_t debounce_ms;

  Input(const char* name, uint8_t pin, uint8_t pin_mode);
  // ~Input();

  virtual void setup();
  virtual void loop();
};


}