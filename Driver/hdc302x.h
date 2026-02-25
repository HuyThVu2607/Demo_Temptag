#ifndef HDC302X_H
#define HDC302X_H

#include <stdint.h>
#include <stdbool.h>

/* ===== I2C address (7-bit) ===== */
#define HDC302X_I2C_ADDR        0x44  

/* ===== Commands ===== */
#define HDC302X_CMD_SOFT_RESET  0x30A2
#define HDC302X_CMD_CLEAR_STAT  0x3041
#define HDC302X_CMD_READ_MFGID  0x3781

typedef enum
{
    HDC302X_TRIGGER_LP0 = 0x2400,
    HDC302X_TRIGGER_LP1 = 0x240B,
    HDC302X_TRIGGER_LP2 = 0x2416,
    HDC302X_TRIGGER_LP3 = 0x24FF
} hdc302x_trigger_mode_t;


bool HDC302X_Init(void);

uint16_t HDC302X_ReadManufacturerID(void);

bool HDC302X_ReadTempRH(float *temp_c,
                        float *rh_percent);

bool HDC302X_ReadTempRH_OnDemand(float *temp_c,
                                 float *rh_percent,
                                 hdc302x_trigger_mode_t mode);

#endif
