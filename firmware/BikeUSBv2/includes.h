#include <EEPROM.h>

#define wdt_reset() __asm__ __volatile__ ("wdr"::)

#define SWI2C_CLOCK 10000
#include "lib/SWI2C_H3/src/SWI2C.h" //https://github.com/H3wastooshort/SWI2C_H3
#include "lib/SWI2C_H3/src/SWI2C.cpp"
