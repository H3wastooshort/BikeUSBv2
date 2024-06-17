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
#include "led.h"
#include "ac.h"
#include "eeprom_util.h"

SoftWire twi(SDA_PIN, SCL_PIN);

void setup() {
  Serial.begin(38400);
  Serial.swap(2);


  //I2C
  twi.begin();
  twi.enablePullups();
  twi.setClock(10000);
}

void loop() {
}
