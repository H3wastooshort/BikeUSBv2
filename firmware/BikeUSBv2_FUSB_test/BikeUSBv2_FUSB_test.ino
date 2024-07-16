#include "../BikeUSBv2/lib/SWI2C_H3/src/SWI2C.h"
#include "../BikeUSBv2/lib/SWI2C_H3/src/SWI2C.cpp"
const uint8_t SDA_PIN = PIN_PC0;
const uint8_t SCL_PIN = PIN_PC1;

const uint8_t FUSB_INT_PIN = PIN_PA4;
SWI2C fusb_i2c(SDA_PIN, SCL_PIN, 0x22);

void setup() {
  // put your setup code here, to run once:
  _PROTECTED_WRITE(WDT.CTRLA, WDT_PERIOD_1KCLK_gc);
  Serial.pins(PIN_PA1, PIN_PA2);
  Serial.begin(9600);
  Serial.println("READY");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    delay(100);
    switch (Serial.read()) {
      case 'R':
        {
          Serial.println("reading...");
          uint8_t reg = 0, dat = 0xFF;

          reg = Serial.parseInt();

          Serial.printHexln(reg);

          Serial.println(fusb_i2c.readFromRegister(reg, dat) ? "OK" : "FAIL");
          Serial.printHexln(dat);
        }
        break;
      case 'W':
        {
          Serial.println("writing...");

          uint8_t reg = 0, dat = 0;
          reg = Serial.parseInt();
          dat = Serial.parseInt();

          Serial.printHexln(reg);
          Serial.printHexln(dat);

          Serial.println(fusb_i2c.writeToRegister(reg, dat) ? "OK" : "FAIL");
        }
        break;
    }
  }

  __asm__ __volatile__("wdr" ::);
}
