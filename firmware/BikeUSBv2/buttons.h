const uint8_t BTN_DEBOUNCE = 100;

void full_reset() {
  _PROTECTED_WRITE(WDT.CTRLA, WDT_PERIOD_8CLK_gc);
  while (1) {}  //do nothing, wait for watchdog
}

void dumb_mode() {
  msm_change_state(MSM_SWITCH_DUMB_MODE);
}

uint32_t last_btn1_down = 0;
uint32_t last_btn2_down = 0;

void btn1_isr() {
  last_btn1_down = millis();
}

void btn2_isr() {
  last_btn2_down = millis();
}

void button_setup() {
  attachInterrupt(BTN1_PIN, btn1_isr, FALLING);
  attachInterrupt(BTN2_PIN, btn2_isr, FALLING);
}

void btn1_action() {
  switch (msm_state) {
    case MSM_CONFIG_MODE:
      config.calibrate_mode = 1;
      storeConfig();
      break;
    default: dumb_mode(); break;
  }
}
void btn2_action() {
  switch (msm_state) {
    case MSM_CONFIG_MODE:
      config.calibrate_mode = 2;
      storeConfig();
      break;
    default: dumb_mode(); break;
  }
}
void btn3_action() {
  switch (msm_state) {
    case MSM_CONFIG_MODE:
      config.calibrate_mode = 3;
      storeConfig();
      break;
    default: break;
  }
}

void button_loop() {
  if (last_btn1_down != 0) {  //if btn down event occurred
    uint32_t b1_dn = millis() - last_btn1_down;
    if (b1_dn > BTN_DEBOUNCE) {   //if event long enough ago
      last_btn1_down = 0;         //clear event
      if (last_btn2_down != 0) {  //both pressed
        last_btn2_down = 0;
        btn3_action();
      } else {  //normal btn press
        btn1_action();
      }
    }
  }
  if (last_btn2_down != 0) {  //if btn down event occurred
    uint32_t b2_dn = millis() - last_btn1_down;
    if (b2_dn > BTN_DEBOUNCE) {   //if event long enough ago
      last_btn2_down = 0;         //clear event
      if (last_btn1_down != 0) {  //both pressed
        last_btn1_down = 0;
        btn3_action();
      } else {  //normal btn press
        btn2_action();
      }
    }
  }
}
