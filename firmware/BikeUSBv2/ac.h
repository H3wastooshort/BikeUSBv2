//conf
const uint16_t AC_MAX_PULSEWIDTH = 2000;  //ms
const uint16_t AC_MIN_PULSEWIDTH = 2;     //500Hz

//code
#include <Comparator.h>

uint32_t last_AC_interrupt = 0;  // in ms
#define AC_PULSEWIDTH_SLOTS 4
uint16_t last_AC_pulsewidths[AC_PULSEWIDTH_SLOTS] = { 0, 0, 0, 0 };  // in ms
uint8_t AC_pulsewidths_index = 0;

uint16_t AC_frequency = 0;
bool AC_new_meas_flag = false;

void AC_interrupt() {
  uint32_t pulsewidth = millis() - last_AC_interrupt;
  if (pulsewidth < AC_MIN_PULSEWIDTH) return;  //ignore

  if (pulsewidth > AC_MAX_PULSEWIDTH) {  // freq too low
    AC_pulsewidths_index = 0;            //start measurement cycle over
    return;
  }

  last_AC_pulsewidths[AC_pulsewidths_index] = pulsewidth;

  AC_pulsewidths_index++;

  if (AC_pulsewidths_index == AC_pulsewidth_SLOTS) {
    uint32_t avg_pulsewidth = 0;
    for (uint8_t i = 0; i < AC_PULSEWIDTH_SLOTS; i++) avg_pulsewidth += last_AC_pulsewidths[i];
    avg_pulsewidth /= AC_pulsewidth_SLOTS;

    AC_frequency = 1000 / avg_pulsewidth;
    AC_new_meas_flag = true;

    AC_pulsewidths_index = 0;
  }
  last_AC_interrupt = millis();
}

void initAC() {
  Comparator.input_p = comparator::in_p::in0;  //change this if you changed the ac measurement pin

  Comparator.output = comparator::out::disable;
  Comparator.input_n = comparator::in_n::vref;
  Comparator.reference = comparator::ref::vref_1v1;  //far enough from 0V and should always be accurate
  Comparator.hysteresis = comparator::hyst::large;
  Comparator.init();
  Comparator.start();
  Comparator.attachInterrupt(AC_interrupt, FALLING);
}
