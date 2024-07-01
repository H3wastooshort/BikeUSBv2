void setPowerOutput(bool state) digitalWrite(OUTPUT_ENABLE_PIN, state);
bool isPowerGood() return digitalRead(OUTPUT_GOOD_PIN);
