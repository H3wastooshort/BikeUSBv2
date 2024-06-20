//EEPROM Integrity Check
#include "lib/crc32.h"
//#include <EEPROM.h>

#define CRC_POS ((EEPROM.length() - 1) - sizeof(crc))

bool EEPROM_checkIntegrity(size_t len = 0XFFFF) {
  uint32_t table[256];
  crc32::generate_table(table);

  uint32_t crc;
  size_t pos = CRC_POS;
  EEPROM.get(pos, crc);

  uint32_t crc2;
  size_t end = min(pos, len);
  for (size_t i = 0; i < end; i++) crc2 = crc32::update(table, crc2, EEPROM.read(i));

  bool ok = crc == crc2;
  printDebug(DBG_EEPROM, crc);
  printDebug(DBG_EEPROM, crc2);
  printDebug(DBG_EEPROM, ok);
  return ok;
}

void EEPROM_markClean() {
  uint32_t table[256];
  crc32::generate_table(table);

  uint32_t crc;
  size_t pos = CRC_POS;
  for (size_t i = 0; i < pos; i++) crc = crc32::update(table, crc, EEPROM.read(i));
  EEPROM.put(pos, crc);

  printDebug(DBG_EEPROM, crc);
}

void EEPROM_markClean(const void* data, size_t len) {
  uint32_t table[256];
  crc32::generate_table(table);

  uint32_t crc;
  size_t pos = CRC_POS;
  //for (size_t i = 0; i < pos; i++) crc = crc32::update(table, crc, EEPROM.read(i));
  crc = crc32::update(table, crc, data, len);
  EEPROM.put(pos, crc);

  printDebug(DBG_EEPROM, crc);
}
