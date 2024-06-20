const uint8_t cal_mode_pattern_2W5[] = { 10, 25, 50, 25 };
const uint8_t cal_mode_pattern_5W[] = { 10, 10, 10, 25, 50, 25 };
const uint8_t cal_mode_pattern_7W5[] = { 10, 10, 10, 10, 10, 25, 50, 25 };

void calibration_setup() {
  switch (config.calibrate_mode) {

    default:
      printDebug(DBG_CALIBRATE, 0xFF);
      config.calibrate_mode = 0;
      storeConfig();
      break;
  }
}


void calibration_loop() {  //runs when in cal mode
}
