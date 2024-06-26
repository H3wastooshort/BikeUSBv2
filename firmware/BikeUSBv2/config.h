struct config_t {
  //mode
  byte calibrate_mode = 0x00;  //0 off, 1 2W5, 2 5W, 3 7W5

  //profiles
  uint16_t min_speed_2W5 = 0xFFFF;
  uint16_t min_speed_5W = 0xFFFF;
  uint16_t min_speed_7W5 = 0xFFFF;
} config;

bool loadConfig() {
  if (!EEPROM_checkIntegrity(sizeof(config))) return false;

  EEPROM.get(0, config);
  return true;
}

void storeConfig() {
  EEPROM.put(0, config);
  EEPROM_markClean(&config, sizeof(config));
}
