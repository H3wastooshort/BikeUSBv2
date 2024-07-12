
void config_setup() {
  pwr_led.setStatic(false);
  usr_led.setStatic(true);
}

void config_loop() {  //runs when in config mode
}

void btn_set_led(uint8_t mode) {
  switch (mode) {
    default:
    case 0: pwr_led.setStatic(true); break;
    case 1: pwr_led.setPattern(cal_mode_pattern_2W5); break;
    case 2: pwr_led.setPattern(cal_mode_pattern_5W); break;
    case 3: pwr_led.setPattern(cal_mode_pattern_7W5); break;
  }
}

void btn_set_conf_mode(uint8_t mode) {
  if (config.calibrate_mode == 0) config.calibrate_mode = mode;
  else config.calibrate_mode = 0;
  storeConfig();
  btn_set_led(mode);
}

