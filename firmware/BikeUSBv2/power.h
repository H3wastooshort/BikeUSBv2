void setPowerOutput(bool state) {digitalWrite(OUTPUT_ENABLE_PIN, state); printDebug(DBG_POWER,state);}
bool isPowerGood() {return digitalRead(OUTPUT_GOOD_PIN);}
