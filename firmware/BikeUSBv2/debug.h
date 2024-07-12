enum debug_code_t {
  DBG_BOOT = 0,
  DBG_EEPROM = 1,
  DBG_RESET,
  DBG_LOOP,

  DBG_CONFIG = 31,
  DBG_CALIBRATE = 32,

  DBG_MSM_STATE = 64,
  DBG_FREQ = 64,

  DBG_PDO = 128,
  DBG_PD_ERR,

  DBG_FUSB_INT = 192
};


template<typename T> void printDebug(debug_code_t code, T val);

void doDebugOutput();
