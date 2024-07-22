template<typename T> void printDebug(debug_code_t code, T val) {
  Serial.println();
  Serial.printHex((uint8_t)code);
  Serial.print("=>");
  Serial.printHex(val);
  Serial.println();
}

void doDebugOutput() {
  if (AC_new_meas_flag) printDebug(DBG_FREQ, AC_frequency);
}

extern int __stack;
extern int __heap_start;
#define RAM_DUMP_CHUNK_SIZE
void dumpRAM() {
  Serial.println();
  Serial.println("RAM DUMP");
  uint8_t* lower = (uint8_t*)&__stack;
  uint8_t* upper = (uint8_t*)&__heap_start;
  Serial.printHex((size_t)lower);
  Serial.print(" to ");
  Serial.printHex((size_t)upper);
  Serial.println();
  Serial.println();
  Serial.printHex(lower, size_t(upper - lower), ':');
  Serial.println();
  Serial.println();
}

void dumpEEPROM() {
  size_t len = EEPROM.length();
  for (size_t b = 0; b < len; b++) {
    Serial.printHex(EEPROM.read(b));
    if (b + 1 < len) Serial.write(':');
  }
}

void Serial_flushRX() {
  while (Serial.available()) Serial.read();
}
void handleSerialCommands() {
  if (!Serial.available()) return;
  char cmd = Serial.read();
  if (cmd != '/') return;
  switch (cmd) {
    case 'd':
      //Dump RAM
      dumpRAM();
      break;
    case 'e':
      //Dump EEPROM
      dumpEEPROM();
      break;
    case 'm':  //Ex. /m3
      //set MSM state
      delay(100);
      msm_change_state(Serial.parseInt());
      break;
    case 's':
      //set SRC state
      delay(100);
      src_change_state(Serial.parseInt());
      break;
    case 'C':  //Ex.: /C200;400;800
      {
        //set SRC state
        delay(100);
        auto i = Serial.parseInt();
        if (i < 0 || i > 0xFFFF) break;
        config.min_speed_2W5 = i;
        delay(100);
        i = Serial.parseInt();
        if (i < 0 || i > 0xFFFF) break;
        config.min_speed_5W = i;
        delay(100);
        i = Serial.parseInt();
        if (i < 0 || i > 0xFFFF) break;
        config.min_speed_7W5 = i;

        config.calibrate_mode = 0;
        storeConfig();
      }
      break;
  }
  //Serial_flushRX();
}
