//====PINS====

const uint8_t OUTPUT_ENABLE_PIN = PIN_PC2;
const uint8_t OUTPUT_GOOD_PIN = PIN_PC3;

const uint8_t AC_MEASURE_PIN = PIN_PA7;  //also AINP0, don't change this or AC code must be changed aswell

const uint8_t SDA_PIN = PIN_PC0;
const uint8_t SCL_PIN = PIN_PC1;

const uint8_t FUSB_INT_PIN = PIN_PA4;

#define HAS_FUSB

const uint8_t BTN1_PIN = PIN_PB3;
const uint8_t BTN2_PIN = PIN_PB4;

const uint8_t USR_LED_PIN = PIN_PB2;
const uint8_t PWR_LED_PIN = PIN_PB3;

const uint8_t CC1_PIN = PIN_PB0;
const uint8_t CC2_PIN = PIN_PB1;
const uint8_t CC_DAC_PIN = PIN_PA6;

//====OTHER====

const uint16_t MIN_INPUT_VOLTAGE = 3000;  //in mV, minimum MCU supply voltage needed to allow operation of device

//============

void setupHardware() {
  //pin modes
  pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
  digitalWrite(OUTPUT_ENABLE_PIN, LOW);
  pinMode(USR_LED_PIN, OUTPUT);

  pinMode(AC_MEASURE_PIN, INPUT);
  pinMode(OUTPUT_GOOD_PIN, INPUT_PULLUP);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);

  pinMode(FUSB_INT_PIN, INPUT_PULLUP);

  //ADC0 / CC Pin DAC
  analogReadResolution(10);
  analogReference(INTERNAL2V5);  //set ADC reference to 2.5V as it covers the full range of CC voltages
  /*//ADC1 / AC Measurement DAC
  analogReadResolution1(8);
  analogReference1(INTERNAL1v1); //set ADC reference to 1.1V as this should always be accurate*/

  //DAC
  DACReference(INTERNAL2V5);  //set DAC reference to 2.5V
}
