const uint8_t cal_mode_pattern_2W5[] = { 10, 25, 50, 25, 0 };
const uint8_t cal_mode_pattern_5W[] = { 10, 10, 10, 25, 50, 25, 0 };
const uint8_t cal_mode_pattern_7W5[] = { 10, 10, 10, 10, 10, 25, 50, 25, 0 };

void calibration_setup() {
  printDebug(DBG_CALIBRATE, config.calibrate_mode);

  switch (config.calibrate_mode) {
    case 0:
      usr_led.setPattern(cal_mode_pattern_2W5);
      break;
    case 1:
      usr_led.setPattern(cal_mode_pattern_5W);
      break;
    case 2:
      usr_led.setPattern(cal_mode_pattern_7W5);
      break;
    default:
      printDebug(DBG_CALIBRATE, 0xFF);
      config.calibrate_mode = 0;
      storeConfig();
      return;
      break;
  }

  digitalWrite(OUTPUT_ENABLE_PIN, HIGH);
}


void calibration_loop() {  //runs when in cal mode
  static uint32_t last_no_pg = 0;
  if (!digitalRead(OUTPUT_GOOD_PIN)) last_no_pg = millis();

  if (last_no_pg != 0 && millis() - last_no_pg > 5000) {
    //save freq
    usr_led.setStatic(true);
    switch (config.calibrate_mode) {
      case 0:
        config.min_speed_2W5 = AC_frequency;
        break;
      case 1:
        config.min_speed_5W = AC_frequency;
        break;
      case 2:
        config.min_speed_7W5 = AC_frequency;
        break;
    }
    if (config.calibrate_mode > 0) {
      config.calibrate_mode = 0;
      storeConfig();
    }
  }
}
