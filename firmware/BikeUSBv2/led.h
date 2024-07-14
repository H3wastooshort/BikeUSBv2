//example pattern: 2s on, 1s, off, 2.5s on, 500ms off => [200,100,250,50]
class BlinkyLED {
protected:
  uint8_t led_pin;

  uint32_t last_led_change = 0;
  uint8_t* pattern = NULL;
  uint8_t pattern_index = 0;
  bool led_state = true;

public:
  BlinkyLED(const uint8_t pin) {
    led_pin = pin;

    last_led_change = 0;
    pattern = NULL;
    pattern_index = 0;
    led_state = true;
  }

  void setup() {
    pinMode(led_pin, OUTPUT);
    pinMode(led_pin, LOW);
  }
  void loop() {
    if (pattern == NULL) return;

    uint16_t step_delay = pattern[pattern_index] * 10;
    if (millis() - last_led_change > step_delay) {  //time reached, go to next step
      pattern_index++;

      if (pattern[pattern_index] == 0 || pattern_index == 0xFF) {
        pattern_index = 0;
        led_state = true;
      } else led_state ^= 1;  //invert

      digitalWrite(led_pin, led_state);
      last_led_change = millis();
    }
  }

  void setPattern(const uint8_t* new_pattern) {
    last_led_change = 0;
    pattern_index = 0;
    pattern = const_cast<uint8_t*>(new_pattern);
  }

  void setStatic(const bool s) {
    setPattern(NULL);
    digitalWrite(led_pin, s);
  }
};

//Patterns
constexpr uint8_t led_pattern_quick_blinky[3] = {10,10,0};
const uint8_t cal_mode_pattern_2W5[] = { 10, 25, 50, 25, 0 };
const uint8_t cal_mode_pattern_5W[] = { 10, 10, 10, 25, 50, 25, 0 };
const uint8_t cal_mode_pattern_7W5[] = { 10, 10, 10, 10, 10, 25, 50, 25, 0 };
const uint8_t cal_mode_pattern_done[] = { 50, 50, 0 };
