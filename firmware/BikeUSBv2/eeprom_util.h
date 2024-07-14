//EEPROM Integrity Check

#define CRC_POS ((EEPROM.length() - 1) - sizeof(uint32_t))

bool EEPROM_checkIntegrity(size_t len = 0xFFFF) {
  uint32_t crc;
  size_t pos = CRC_POS;
  EEPROM.get(pos, crc);


  CRC32 crc32_inst;
  crc32_inst.restart();
  size_t end = min(pos, len);
  for (size_t i = 0; i < end; i++) crc32_inst.add(EEPROM.read(i));
  uint32_t crc2 = crc32_inst.calc();

  bool ok = crc == crc2;
  printDebug(DBG_EEPROM, crc);
  printDebug(DBG_EEPROM, crc2);
  printDebug(DBG_EEPROM, ok);
  return ok;
}

void EEPROM_markClean(size_t len = 0xFFFF) {
  CRC32 crc32_inst;
  crc32_inst.restart();
  size_t pos = CRC_POS;
  size_t end = min(pos, len);
  for (size_t i = 0; i < end; i++) crc32_inst.add(EEPROM.read(i));

  uint32_t crc = crc32_inst.calc();
  EEPROM.put(pos, crc);

  printDebug(DBG_EEPROM, crc);
}

void EEPROM_markClean(const void* data, size_t len) {
  CRC32 crc32_inst;
  crc32_inst.restart();
  size_t pos = CRC_POS;
  crc32_inst.add(data, len);

  uint32_t crc = crc32_inst.calc();
  EEPROM.put(pos, crc);

  printDebug(DBG_EEPROM, crc);
}
