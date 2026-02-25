#ifndef FM13DT160_H
#define FM13DT160_H

#include <stdint.h>
#include <stdbool.h>

/* ===== I2C address (7-bit for MSPM0) ===== */
#define FM13DT160_I2C_ADDR   0x48  

/* ===== Commands ===== */
#define CMD_WRITE_MEM  0xB3
#define CMD_READ_MEM   0xB1

/* ===== API ===== */

void FM13DT160_PowerOn(void);
void FM13DT160_PowerOff(void);

bool FM13DT160_WriteEEPROM(uint16_t addr,
                           const uint8_t *pData,
                           uint16_t size);

bool FM13DT160_ReadEEPROM(uint16_t addr,
                          uint8_t *pData,
                          uint16_t size);

#endif
