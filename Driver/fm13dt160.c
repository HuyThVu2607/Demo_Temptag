#include "fm13dt160.h"
#include "mspm0_i2c_polling_safe.h"
#include <string.h>

uint8_t rx_dummy[100];

void FM13DT160_PowerOn(void)
{
    DL_GPIO_clearPins(GROUP0_PORT, GROUP0_PWR_OUT_PIN);
    //DL_GPIO_setPins(GROUP0_PORT, GROUP0_PWR_OUT_PIN);
    delay_cycles(240000);
}

void FM13DT160_PowerOff(void)
{
    DL_GPIO_setPins(GROUP0_PORT, GROUP0_PWR_OUT_PIN);
    //DL_GPIO_clearPins(GROUP0_PORT, GROUP0_PWR_OUT_PIN);
    delay_cycles(2400000);
}

/* ================= CRC16 ================= */
static uint16_t FM13DT160_CRC16(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFF;

    for (uint16_t i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }
    return crc;
}

/* ================= EEPROM WRITE ================= */

bool FM13DT160_WriteEEPROM(uint16_t addr,
                           const uint8_t *pData,
                           uint16_t size)
{
    uint8_t buffer[64];

    /* Frame:
     * [0] CMD
     * [1] Addr MSB
     * [2] Addr LSB
     * [3] Length
     * [4..] Data
     * [..] CRC16 LSB
     * [..] CRC16 MSB
     */

    buffer[0] = CMD_WRITE_MEM;
    buffer[1] = (uint8_t)(addr >> 8);
    buffer[2] = (uint8_t)(addr & 0xFF);
    buffer[3] = (uint8_t)size;

    memcpy(&buffer[4], pData, size);

    uint16_t crc = FM13DT160_CRC16(buffer, size + 3);

    buffer[size + 4] = (uint8_t)(crc & 0xFF);
    buffer[size + 5] = (uint8_t)(crc >> 8);

    /* Total length = size + 6 */
    if (!MSPM0_I2C_Write(
            FM13DT160_I2C_ADDR,
            buffer,
            size + 6))
        return false;
    delay_cycles(240000);
    /* Read & discard ACK / response frame */
    MSPM0_I2C_Read(
        FM13DT160_I2C_ADDR,
        rx_dummy,
        sizeof(rx_dummy));

    return true;
}

/* ================= EEPROM READ ================= */

bool FM13DT160_ReadEEPROM(uint16_t addr,
                          uint8_t *pData,
                          uint16_t size)
{
    uint8_t cmd_buf[7];

    /* Frame:
     * [0] CMD
     * [1] Addr MSB
     * [2] Addr LSB
     * [3] Len MSB
     * [4] Len LSB
     * [5] CRC LSB
     * [6] CRC MSB
     */

    cmd_buf[0] = CMD_READ_MEM;
    cmd_buf[1] = (uint8_t)(addr >> 8);
    cmd_buf[2] = (uint8_t)(addr & 0xFF);
    cmd_buf[3] = (uint8_t)(size >> 8);
    cmd_buf[4] = (uint8_t)(size & 0xFF);

    uint16_t crc = FM13DT160_CRC16(cmd_buf, 5);
    cmd_buf[5] = (uint8_t)(crc & 0xFF);
    cmd_buf[6] = (uint8_t)(crc >> 8);

    if (!MSPM0_I2C_Write(
            FM13DT160_I2C_ADDR,
            cmd_buf,
            sizeof(cmd_buf)))
        return false;

    /* Datasheet requires delay after READ command */
    delay_cycles(2400000);   /* ~10ms */

    if (!MSPM0_I2C_Read(
            FM13DT160_I2C_ADDR,
            pData,
            size))
        return false;

    return true;
}
