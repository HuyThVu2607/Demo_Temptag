#include "acc_sc7a20.h"
#include "mspm0_i2c_polling_safe.h"

/* ===== Low-level helpers ===== */

static bool I2C_WriteReg(uint8_t reg, uint8_t data)
{
    uint8_t buf[2] = { reg, data };
    return MSPM0_I2C_Write(SC7A20H_I2C_ADDR, buf, 2);
}

static bool I2C_ReadRegs(uint8_t reg, uint8_t *buf, uint8_t len)
{
    return MSPM0_I2C_WriteRead(
        SC7A20H_I2C_ADDR,
        &reg, 1,
        buf, len);
}

/* ===== Public API ===== */

uint8_t SC7A20H_ReadWhoAmI(void)
{
    uint8_t id = 0;
    I2C_ReadRegs(SC7A20H_WHO_AM_I, &id, 1);
    return id;
}

bool SC7A20H_Init(void)
{
    /* Power-up delay */
    delay_cycles(24000); /* ~1ms */

    /* CTRL1:
     * ODR = 100Hz
     * Enable X/Y/Z
     */
    if (!I2C_WriteReg(SC7A20H_CTRL1, 0x57))
        return false;

    /* CTRL4:
     * ±2g
     * High resolution
     */
    if (!I2C_WriteReg(SC7A20H_CTRL4, 0x08))
        return false;

    return true;
}

bool SC7A20H_ReadXYZ(int16_t *x,
                     int16_t *y,
                     int16_t *z)
{
    uint8_t buf[6];
    uint8_t reg = SC7A20H_OUT_X_L | 0x80; /* auto-increment */

    if (!I2C_ReadRegs(reg, buf, 6))
        return false;

    *x = (int16_t)((buf[1] << 8) | buf[0]);
    *y = (int16_t)((buf[3] << 8) | buf[2]);
    *z = (int16_t)((buf[5] << 8) | buf[4]);

    return true;
}

bool SC7A20H_EnableMotionInt(void)
{
    /* CTRL2:
     * HPF enable
     * Normal mode
     * HPF for INT1
     */
    if (!I2C_WriteReg(SC7A20H_CTRL2, 0x01))
        return false;

    /* Reset HPF reference*/
    uint8_t dummy1;
    I2C_ReadRegs(SC7A20H_REFERENCE, &dummy1, 1);

    /* INT1_CFG:
     * OR combination
     * XHIE + YHIE 
     */
    if (!I2C_WriteReg(SC7A20H_INT1_CFG, 0x0A))
        return false;

    /* Threshold ~100 mg */
    if (!I2C_WriteReg(SC7A20H_INT1_THS, 0x06))
        return false;

    /* Duration = 0  */
    if (!I2C_WriteReg(SC7A20H_INT1_DUR, 0x00))
        return false;

    /* Route INT1 */
    if (!I2C_WriteReg(SC7A20H_CTRL3, 0x40))
        return false;

    /* Latch interrupt */
    if (!I2C_WriteReg(SC7A20H_CTRL5, 0x08))
        return false;

    return true;
}

uint8_t SC7A20H_ReadIntSource(void)
{
    uint8_t src = 0;
    I2C_ReadRegs(SC7A20H_INT1_SRC, &src, 1);
    return src;
}