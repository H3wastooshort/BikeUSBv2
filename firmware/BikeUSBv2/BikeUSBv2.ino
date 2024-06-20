/*
megaTinyCore Board Config:
BOD Mode = Active / Sampled (125Hz)
BOD Voltage Level = 1.8V
Chip = ATtiny 1616
Clock = 4Mhz Internal 
millis()/micros() = Enabled (default timer) or maybe RTC (no micros). not sure yet
printf() = Minimal
Startup Time = 64ms
Programmer: SerialUPDI - 230400 baud
*/

#include "includes.h"
#include "hardware.h"
#include "debug.h"
#include "eeprom_util.h"
#include "config.h"
#include "main_state_machine.h"
#include "led.h"
#include "ac.h"
#include "power_levels.h"

BlinkyLED usr_led(USR_LED_PIN);
BlinkyLED pwr_led(PWR_LED_PIN);
SoftWire twi(SDA_PIN, SCL_PIN);

#include "debug.2.h"
#include "config_mode.h"
#include "calibrate_mode.h"
#include "main_state_machine.2.h"


void setup() {
  setupHardware();
  usr_led.setup();
  usr_led.setStatic(true);
  pwr_led.setup();

  Serial.begin(38400);
  Serial.swap(2);
  printDebug(DBG_BOOT, 0x00);

  //load conf
  loadConfig();
  printDebug(DBG_BOOT, 0x01);

  //I2C
  twi.begin();
  twi.enablePullups();
  twi.setClock(10000);
  printDebug(DBG_BOOT, 0x02);

  usr_led.setStatic(false);
}

void loop() {

  usr_led.loop();
  pwr_led.loop();
  doDebugOutput();
  AC_new_meas_flag = false;  //anything using this should have run by now
}
