#include "hdc302x.h"
#include "mspm0_i2c_polling_safe.h"

/* ===== CRC-8 (poly 0x31, init 0xFF) ===== */
static uint8_t HDC302X_CalcCRC8(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0xFF;

    for (uint8_t i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (uint8_t b = 0; b < 8; b++)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc <<= 1;
        }
    }
    return crc;
}

static bool HDC302X_WriteCmd(uint16_t cmd)
{
    uint8_t buf[2];
    buf[0] = (uint8_t)(cmd >> 8);
    buf[1] = (uint8_t)(cmd & 0xFF);

    return MSPM0_I2C_Write(HDC302X_I2C_ADDR, buf, 2);
}

/* ===== PUBLIC API ===== */

bool HDC302X_Init(void)
{
    /* Power-up delay */
    delay_cycles(24000); /* ~1ms @24MHz */

    if (!HDC302X_WriteCmd(HDC302X_CMD_SOFT_RESET))
        return false;

    delay_cycles(24000);

    HDC302X_WriteCmd(HDC302X_CMD_CLEAR_STAT);

    return true;
}

uint16_t HDC302X_ReadManufacturerID(void)
{
    uint8_t tx[2];
    uint8_t rx[3];

    tx[0] = (uint8_t)(HDC302X_CMD_READ_MFGID >> 8);
    tx[1] = (uint8_t)(HDC302X_CMD_READ_MFGID & 0xFF);

    if (!MSPM0_I2C_WriteRead(
            HDC302X_I2C_ADDR,
            tx, 2,
            rx, 3))
        return 0;

    if (HDC302X_CalcCRC8(rx, 2) != rx[2])
        return 0;

    return ((uint16_t)rx[0] << 8) | rx[1];
}

bool HDC302X_ReadTempRH(float *temp_c,
                        float *rh_percent)
{
    return HDC302X_ReadTempRH_OnDemand(
        temp_c, rh_percent, HDC302X_TRIGGER_LP0);
}

bool HDC302X_ReadTempRH_OnDemand(float *temp_c,
                                 float *rh_percent,
                                 hdc302x_trigger_mode_t mode)
{
    uint8_t rx[6];
    uint16_t rawT, rawRH;

    if (!HDC302X_WriteCmd((uint16_t)mode))
        return false;

    /* t_meas LP0 ≈ 1.6ms*/
    delay_cycles(72000);

    if (!MSPM0_I2C_Read(
            HDC302X_I2C_ADDR,
            rx, 6))
        return false;

    if (HDC302X_CalcCRC8(&rx[0], 2) != rx[2])
        return false;

    if (HDC302X_CalcCRC8(&rx[3], 2) != rx[5])
        return false;

    rawT  = ((uint16_t)rx[0] << 8) | rx[1];
    rawRH = ((uint16_t)rx[3] << 8) | rx[4];

    *temp_c = -45.0f + (175.0f * rawT) / 65535.0f;
    *rh_percent = (100.0f * rawRH) / 65535.0f;

    return true;
}
