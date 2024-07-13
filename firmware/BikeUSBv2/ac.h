//conf
const uint16_t AC_MAX_PULSEWIDTH = 2000;  //in ms
const uint16_t AC_MIN_PULSEWIDTH = 2;     //in ms, equals 500Hz

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
  last_AC_interrupt = millis();
  //printDebug(DBG_AC, pulsewidth);

  if (pulsewidth > AC_MAX_PULSEWIDTH) {  // freq too low
    AC_pulsewidths_index = 0;            //start measurement cycle over
    return;
  }

  last_AC_pulsewidths[AC_pulsewidths_index] = pulsewidth;

  AC_pulsewidths_index++;

  if (AC_pulsewidths_index == AC_PULSEWIDTH_SLOTS) {
    uint32_t avg_pulsewidth = 0;
    for (uint8_t i = 0; i < AC_PULSEWIDTH_SLOTS; i++) avg_pulsewidth += last_AC_pulsewidths[i];
    avg_pulsewidth /= AC_PULSEWIDTH_SLOTS;

    AC_frequency = 1000 / avg_pulsewidth;
    AC_new_meas_flag = true;

    AC_pulsewidths_index = 0;
  }
}

void initAC() {
  Comparator.input_p = comparator::in_p::in0;  //change this if you changed the ac measurement pin

  Comparator.output = comparator::out::disable;
  Comparator.input_n = comparator::in_n::dacref;
  Comparator.dacref = 32;  //v_thesh = 1.1 * (32/256) * 10 = 1.375V
  Comparator.reference = comparator::ref::vref_1v1;
  Comparator.hysteresis = comparator::hyst::medium;  // equal to 0.25V scaled
  Comparator.init();
  Comparator.start();
  Comparator.attachInterrupt(AC_interrupt, FALLING);
}
