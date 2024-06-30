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
//#define FUSB_DEBUG_SERIAL Serial
#include "lib/PD_Friend/i2c/swi2c.h"
#include "lib/PD_Friend/pd_friend.h"
#include "debug.h"
#include "eeprom_util.h"
#include "config.h"
#include "main_state_machine.h"
#include "led.h"
#include "ac.h"
#include "power_levels.h"

BlinkyLED usr_led(USR_LED_PIN);
BlinkyLED pwr_led(PWR_LED_PIN);
SWI2C fusb_i2c(SDA_PIN, SCL_PIN, FUSB302_I2C_SLAVE_ADDR);
PDFriendI2C fusb_i2c_compat(fusb_i2c);
FUSB302 fusb(fusb_i2c_compat);
PDStack_SRC pd(fusb);

#include "pd_src.2.h"
#include "debug.2.h"
#include "config_mode.h"
#include "calibrate_mode.h"
#include "main_state_machine.2.h"

void btn1_isr() {  //idk
}

void btn2_isr() {  //reset MSM
}

void setup() {
  wdt_reset();

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
  fusb_i2c.begin();
  printDebug(DBG_BOOT, 0x02);

  //PD
  pd.init_src();

  //buttons
  attachInterrupt(BTN1_PIN, btn1_isr, FALLING);
  attachInterrupt(BTN2_PIN, btn2_isr, FALLING);

  initAC();

  usr_led.setStatic(false);

  wdt_reset();
}

void loop() {
  run_msm();
  run_pd();

  usr_led.loop();
  pwr_led.loop();
  doDebugOutput();
  AC_new_meas_flag = false;  //anything using this should have run by now

  wdt_reset();
}
