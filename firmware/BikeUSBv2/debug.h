enum debug_code_t {
  DBG_BOOT = 0,
  DBG_EEPROM = 1,

  //live data
  DBG_MSM_STATE = 64,
  DBG_FREQ = 64
};


template<typename T> void printDebug(debug_code_t code, T val);

void doDebugOutput();