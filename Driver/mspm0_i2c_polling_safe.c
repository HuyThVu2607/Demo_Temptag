#include "mspm0_i2c_polling_safe.h"

/* ===== INTERNAL HELPERS ===== */

static inline void wait_idle(void)
{
    while (!(DL_I2C_getControllerStatus(I2C_INST) &
             DL_I2C_CONTROLLER_STATUS_IDLE));
}

static inline bool has_error(void)
{
    return (DL_I2C_getControllerStatus(I2C_INST) &
            DL_I2C_CONTROLLER_STATUS_ERROR);
}

/* ===== PUBLIC API ===== */

bool MSPM0_I2C_Write(uint8_t addr,
                     const uint8_t *buf,
                     uint16_t len)
{
    uint16_t i = 0;

    wait_idle();

    /* CRITICAL: Pre-fill TX FIFO BEFORE start */
    while (i < len &&
           !DL_I2C_isControllerTXFIFOFull(I2C_INST))
    {
        DL_I2C_transmitControllerData(I2C_INST, buf[i++]);
    }

    DL_I2C_startControllerTransfer(
        I2C_INST,
        addr,
        DL_I2C_CONTROLLER_DIRECTION_TX,
        len);

    /* Feed remaining bytes */
    while (i < len)
    {
        if (!DL_I2C_isControllerTXFIFOFull(I2C_INST))
        {
            DL_I2C_transmitControllerData(I2C_INST, buf[i++]);
        }
    }

    /* Wait transfer done */
    while (DL_I2C_getControllerStatus(I2C_INST) &
           DL_I2C_CONTROLLER_STATUS_BUSY_BUS);

    return !has_error();
}

bool MSPM0_I2C_Read(uint8_t addr,
                    uint8_t *buf,
                    uint16_t len)
{
    wait_idle();

    DL_I2C_startControllerTransfer(
        I2C_INST,
        addr,
        DL_I2C_CONTROLLER_DIRECTION_RX,
        len);

    for (uint16_t i = 0; i < len; i++)
    {
        while (DL_I2C_isControllerRXFIFOEmpty(I2C_INST));
        buf[i] = DL_I2C_receiveControllerData(I2C_INST);
    }

    return !has_error();
}

bool MSPM0_I2C_WriteRead(uint8_t addr,
                         const uint8_t *tx,
                         uint16_t tx_len,
                         uint8_t *rx,
                         uint16_t rx_len)
{
    if (!MSPM0_I2C_Write(addr, tx, tx_len))
        return false;

    /* Small gap – many sensors expect this */
    __NOP(); __NOP(); __NOP();

    return MSPM0_I2C_Read(addr, rx, rx_len);
}

/* ===== BUS RECOVERY (OPTIONAL) ===== */
void MSPM0_I2C_BusRecover(void)
{
    /* Only call when bus is stuck */

    DL_I2C_disableController(I2C_INST);

    /* TODO:
       - Re-configure SCL as GPIO output
       - Toggle 9 pulses
       - Restore I2C function
       (Implement if needed)
    */

    DL_I2C_enableController(I2C_INST);
}
