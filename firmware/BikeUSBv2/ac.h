//conf
const uint16_t AC_MAX_PULSEWIDTH = 2000;                //in ms
const uint16_t MIN_PW_FOR_PW_METHOD = 50 /*20Hz*/;      //in ms
const uint16_t MIN_FREQ_FOR_CNT_METHOD = 10 /*100ms*/;  //in Hz
const uint16_t AC_counter_interval = 1000;



#include <Comparator.h>
bool AC_pw_method = true;
uint16_t AC_frequency = 0;
bool AC_new_meas_flag = false;


//Counter Method
uint32_t AC_counter = 0;
uint32_t last_AC_counter_calc = 0;

//pulsewidth method
uint32_t last_AC_interrupt = 0;  // in ms
#define AC_PULSEWIDTH_SLOTS 4
uint16_t last_AC_pulsewidths[AC_PULSEWIDTH_SLOTS] = { 0, 0, 0, 0 };  // in ms
uint8_t AC_pulsewidths_index = 0;

void AC_interrupt_pulsewidth() {
  uint32_t pulsewidth = millis() - last_AC_interrupt;
  last_AC_interrupt = millis();
  //printDebug(DBG_AC, pulsewidth);

  //method switching
  if (AC_pw_method) {
    if (pulsewidth < MIN_PW_FOR_PW_METHOD) {
      AC_pw_method = false;
      AC_counter = 1;
      last_AC_counter_calc = millis();
      printDebug(DBG_FREQ_METHOD, AC_pw_method);
      return;
    }
  }

  //calc
  if (pulsewidth > AC_MAX_PULSEWIDTH) {  // freq too low
    AC_pulsewidths_index = 0;            //start measurement cycle over
    return;
  }

  last_AC_pulsewidths[AC_pulsewidths_index] = pulsewidth;

  AC_pulsewidths_index++;

  if (AC_pulsewidths_index >= AC_PULSEWIDTH_SLOTS) {
    AC_pulsewidths_index = 0;

    uint32_t avg_pulsewidth = 0;
    for (uint8_t i = 0; i < AC_PULSEWIDTH_SLOTS; i++) avg_pulsewidth += last_AC_pulsewidths[i];
    avg_pulsewidth /= AC_PULSEWIDTH_SLOTS;

    AC_frequency = 1000 / avg_pulsewidth;
    AC_new_meas_flag = true;
  }
}


//Counter Method
void AC_loop_counter() {
  if (!AC_pw_method) {
    uint32_t time_passed = millis() - last_AC_counter_calc;
    if (time_passed > AC_counter_interval) {
      float timebase = float(time_passed) / 1000.0;
      AC_frequency = AC_counter / timebase;

      printDebug(DBG_AC, AC_counter);
      printDebug(DBG_AC, time_passed);

      last_AC_counter_calc = millis();
      AC_counter = 0;
      AC_new_meas_flag = true;

      if (AC_frequency < MIN_FREQ_FOR_CNT_METHOD) {
        AC_pw_method = true;
        printDebug(DBG_FREQ_METHOD, AC_pw_method);
      }
    }
  }
}

//Interrupt
void AC_interrupt() {
  AC_counter++;
  if (AC_pw_method) AC_interrupt_pulsewidth();
}


void initAC() {
  Comparator.input_p = comparator::in_p::in0;  //change this if you changed the ac measurement pin

  Comparator.output = comparator::out::disable;
  Comparator.input_n = comparator::in_n::dacref;
  Comparator.dacref = 48;  //v_thesh = 1.1 * (48/256) * 10 = 2.0625V
  Comparator.reference = comparator::ref::vref_1v1;
  Comparator.hysteresis = comparator::hyst::large;  // equal to 0.5V scaled
  Comparator.init();
  Comparator.start();
  Comparator.attachInterrupt(AC_interrupt, FALLING);
}
