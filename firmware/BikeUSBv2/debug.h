enum debug_code_t {
  DBG_BOOT = 0,
  DBG_EEPROM = 1,

  //modes
  DBG_CONFIG = 31,
  DBG_CALIBRATE = 32,

  //live data
  DBG_MSM_STATE = 64,
  DBG_FREQ = 64,

  //PD
  DBG_PDO = 128
};


template<typename T> void printDebug(debug_code_t code, T val);

void doDebugOutput();
