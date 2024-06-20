template<typename T> void printDebug(debug_code_t code, T val) {
  Serial.println();
  Serial.printHex(code);
  Serial.print(" => ");
  Serial.printHex(val);
  Serial.println();
}

void doDebugOutput() {
  if (AC_new_meas_flag) printDebug(DBG_FREQ, AC_frequency);
}
