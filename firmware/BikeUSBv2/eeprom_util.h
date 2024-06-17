//EEPROM Integrity Check

#define CRC_POS EEPROM.length() - 1 - sizeof(crc)

bool EEPROM_checkIntegrity() {
  uint32_t crc;
  size_t pos = CRC_POS;
  EEPROM.get(pos, crc);
  for (size_t i = 0; i < pos; i++) {
    
  }
}

void EEPROM_markClean() {
  uint32_t crc;
  size_t pos = CRC_POS;
  for (size_t i = 0; i < pos; i++) {

  }
  EEPROM.put(pos, crc);
}
