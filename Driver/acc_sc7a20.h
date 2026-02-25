#ifndef ACC_SC7A20_H
#define ACC_SC7A20_H

#include <stdint.h>
#include <stdbool.h>

/* ===== I2C address (7-bit for MSPM0) ===== */
#define SC7A20H_I2C_ADDR   0x19

/* ===== Registers ===== */
#define SC7A20H_WHO_AM_I   0x0F
#define SC7A20H_CTRL1     0x20
#define SC7A20H_CTRL4     0x23

#define SC7A20H_OUT_X_L   0x28
#define SC7A20H_OUT_X_H   0x29
#define SC7A20H_OUT_Y_L   0x2A
#define SC7A20H_OUT_Y_H   0x2B
#define SC7A20H_OUT_Z_L   0x2C
#define SC7A20H_OUT_Z_H   0x2D

/* ===== Interrupt registers ===== */
#define SC7A20H_CTRL3      0x22   // INT1 config
#define SC7A20H_CTRL5      0x24   // latch
#define SC7A20H_INT1_THS   0x32
#define SC7A20H_INT1_DUR   0x33
#define SC7A20H_INT1_CFG   0x30
#define SC7A20H_INT1_SRC   0x31   // MUST read to clear INT

#define SC7A20H_REFERENCE  0x26

/* ===== Interrupt registers ===== */
#define SC7A20H_CTRL2        0x21

/* ===== API (GIỮ GIỐNG STM32) ===== */

bool SC7A20H_Init(void);
uint8_t SC7A20H_ReadWhoAmI(void);

bool SC7A20H_ReadXYZ(int16_t *x,
                     int16_t *y,
                     int16_t *z);

bool SC7A20H_EnableMotionInt(void);
uint8_t SC7A20H_ReadIntSource(void);         

#endif
