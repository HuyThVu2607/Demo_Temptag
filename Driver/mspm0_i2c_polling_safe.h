#ifndef MSPM0_I2C_POLLING_SAFE_H
#define MSPM0_I2C_POLLING_SAFE_H

#include <stdint.h>
#include <stdbool.h>
#include "ti_msp_dl_config.h"

/* ===== API ===== */

bool MSPM0_I2C_Write(uint8_t addr,
                     const uint8_t *buf,
                     uint16_t len);

bool MSPM0_I2C_Read(uint8_t addr,
                    uint8_t *buf,
                    uint16_t len);

bool MSPM0_I2C_WriteRead(uint8_t addr,
                         const uint8_t *tx,
                         uint16_t tx_len,
                         uint8_t *rx,
                         uint16_t rx_len);

/* Optional recovery */
void MSPM0_I2C_BusRecover(void);

#endif
