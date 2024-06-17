//====PINS====

const uint8_t OUTPUT_ENABLE_PIN = PIN_PC2;
const uint8_t OUTPUT_GOOD_PIN = PIN_PC3;

const uint8_t AC_MEASURE_PIN = PIN_PA7; //also AINP0, don't change this or AC code must be changed aswell


//====OTHER====
const uint16_t MIN_INPUT_VOLTAGE = 3000; //in mV, minimum MCU supply voltage needed to allow operation of device


//============

void setupHardware() {
  //pin modes
  pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
  pinMode(OUTPUT_GOOD_PIN, INPUT_PULLUP);
  pinMode(AC_MEASURE_PIN, INPUT);

  //ADC0 / CC Pin DAC
  analogReadResolution(10);
  analogReference(INTERNAL2V5);//set ADC reference to 2.5V as it covers the full range of CC voltages
  /*//ADC1 / AC Measurement DAC
  analogReadResolution1(8);
  analogReference1(INTERNAL1v1); //set ADC reference to 1.1V as this should always be accurate*/

  //DAC
  DACReference(INTERNAL2V5);//set DAC reference to 2.5V
}
