//conf
const uint16_t AC_MAX_PULSEWIDTH = 2000;  //in ms
const uint16_t AC_MIN_PULSEWIDTH = 500;   //in us, quals 0.5ms equals 2000Hz

//code
#include <Comparator.h>

uint32_t last_AC_interrupt = 0;  // in ms
#define AC_PULSEWIDTH_SLOTS 4
uint32_t last_AC_pulsewidths[AC_PULSEWIDTH_SLOTS] = { 0, 0, 0, 0 };  // in ms
uint8_t AC_pulsewidths_index = 0;

uint16_t AC_frequency = 0;
bool AC_new_meas_flag = false;

void AC_interrupt() {
  uint32_t pulsewidth = micros() - last_AC_interrupt;
  if (pulsewidth < AC_MIN_PULSEWIDTH) return;  //ignore
  last_AC_interrupt = micros();
  //printDebug(DBG_AC, pulsewidth);

  if (pulsewidth > (AC_MAX_PULSEWIDTH * 1E3)) {  // freq too low
    AC_pulsewidths_index = 0;                    //start measurement cycle over
    return;
  }

  last_AC_pulsewidths[AC_pulsewidths_index] = pulsewidth;

  AC_pulsewidths_index++;

  if (AC_pulsewidths_index >= AC_PULSEWIDTH_SLOTS) {
    AC_pulsewidths_index = 0;

    uint64_t avg_pulsewidth = 0;
    for (uint8_t i = 0; i < AC_PULSEWIDTH_SLOTS; i++) avg_pulsewidth += last_AC_pulsewidths[i];
    avg_pulsewidth /= AC_PULSEWIDTH_SLOTS;

    AC_frequency = 1E6 / avg_pulsewidth;
    AC_new_meas_flag = true;
  }
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
