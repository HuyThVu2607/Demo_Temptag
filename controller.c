/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "Driver/hdc302x.h"
#include "Driver/acc_sc7a20.h"
#include "Driver/fm13dt160.h"
#include "Driver/SEGGER_RTT.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

/* ========================= */
/* NFC NDEF CONFIG           */
/* ========================= */

#define NREC            10
#define TEXT_LEN        11                          // "00.0C 00.0%"
#define LANG_LEN        2
#define PAYLOAD_LEN     (1 + LANG_LEN + TEXT_LEN)   // STATUS + "en" + text = 14
#define REC_LEN         (4 + PAYLOAD_LEN)           // header(4) + payload
#define NDEF_LEN        (NREC * REC_LEN)

static uint8_t ndef_buf[256];   
static uint16_t ndef_total = 0;
static uint8_t current_record = 0;
static uint8_t verify_buf[32]; 

bool gAccIntFlag  = false;
bool gTempIntFlag = false;
/* ========================= */
/* MAIN                      */
/* ========================= */
void NDEF_Template_Init(void)
{
    uint16_t i = 0;

    ndef_buf[i++] = 0x03;                 // TLV
    ndef_buf[i++] = (uint8_t)NDEF_LEN;    // TLV length

    for (int r = 0; r < NREC; r++)
    {
        uint8_t hdr = (r == 0) ? 0x91 : (r == (NREC-1) ? 0x51 : 0x11);

        ndef_buf[i++] = hdr;
        ndef_buf[i++] = 0x01;
        ndef_buf[i++] = (uint8_t)PAYLOAD_LEN;
        ndef_buf[i++] = 0x54;   // 'T'

        ndef_buf[i++] = 0x02;
        ndef_buf[i++] = 'e';
        ndef_buf[i++] = 'n';

        const char *tpl = "00.0C 00.0%";
        for (int k = 0; k < TEXT_LEN; k++)
            ndef_buf[i++] = tpl[k];
    }

    ndef_buf[i++] = 0xFE;   // terminator
    ndef_total = i;
}

void NDEF_Update_Record(uint8_t r, float temp, float rh)
{
    if (r >= NREC) return;

    int t = (int)(temp * 10.0f);
    int h = (int)(rh * 10.0f);

    if (t < 0) t = 0; if (t > 999) t = 999;
    if (h < 0) h = 0; if (h > 999) h = 999;

    uint16_t rec_start  = 2 + (uint16_t)r * REC_LEN;
    uint16_t text_start = rec_start + 7;

    ndef_buf[text_start + 0] = (t/100) + '0';
    ndef_buf[text_start + 1] = ((t/10)%10) + '0';
    ndef_buf[text_start + 3] = (t%10) + '0';

    ndef_buf[text_start + 6] = (h/100) + '0';
    ndef_buf[text_start + 7] = ((h/10)%10) + '0';
    ndef_buf[text_start + 9] = (h%10) + '0';
}

void NDEF_Flush_All(void)
{
    uint16_t addr = 0x0010;
    uint16_t i = 0;
    uint16_t len = sizeof(ndef_buf);

    while (i < len)
    {
        uint8_t w = (len - i >= 3) ? 3 : (len - i);

        FM13DT160_PowerOn();
        FM13DT160_WriteEEPROM(addr, &ndef_buf[i], w);
        delay_cycles(100000);
        FM13DT160_PowerOff();
        delay_cycles(100000);

        addr += w;
        i += w;
    }
}

void fm13_write_hello(void)
{
    /* ---------- CC FILE (Type 2) ---------- */
    uint8_t ndef[] = {
        // CC FILE (Type2)
        0x1D, 0xB9, 0x9B, 0xB7,  // Block 0
        0x9C, 0x00, 0x00, 0x70,  // Block 1
        0xEC, 0x00, 0x00, 0x00,  // Block 2
        0xE1, 0x10, 0x60, 0x00   // Block 3 (CC)
    };

    uint8_t uid_cc_hw3[16] =
    {
        0x1D, 0x26, 0x73, 0xC0,
        0x9C, 0x00, 0x00, 0x70,
        0xEC, 0x00, 0x00, 0x00,
        0xE1, 0x10, 0x60, 0x00
    };

    uint8_t uid_cc_hw3_rst[16] =
    {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };

    uint8_t ndef_type4[] =
    {
        /* Block 4 - TLV */
        0x03, 0x0C,

        /* Block 5 - NDEF Header */
        0xD1, 0x01, 0x07, 0x54,

        /* Block 6 */
        0x02, 'e', 'n', 'H',

        /* Block 7 */
        'e', 'l', 'l', 'o',

        /* Block 8 */
        0xFE, 0x00
    };

    uint8_t ndef_2_records[] = {

        /* Block 4 */
        0x03, 0x18,             // TLV length = 24 bytes

        /* Record 1 */
        0x91,                   // MB=1 ME=0 SR=1 TNF=1
        0x01,                   // type length
        0x08,                   // payload length
        0x54,                   // 'T'
        0x02,                   // lang length
        0x65, 0x6E,             // "en"
        0x48, 0x65, 0x6C, 0x6C, 0x6F,   // HELLO

        /* Record 2 */
        0x51,                   // MB=0 ME=1 SR=1 TNF=1
        0x01,                   // type length
        0x08,                   // payload length
        0x54,
        0x02,
        0x65, 0x6E,
        0x57, 0x4F, 0x52, 0x4C, 0x44,   // WORLD

        /* Terminator */
        0xFE

    };

    uint16_t addr = 0x0010;
    uint16_t i = 0;
    uint16_t len = sizeof(ndef_2_records);

    while (i < len)
    {
        uint8_t w = (len - i >= 3) ? 3 : (len - i);

        FM13DT160_PowerOn();
        FM13DT160_WriteEEPROM(addr, &ndef_2_records[i], w);
        FM13DT160_PowerOff();
        delay_cycles(240000);

        addr += w;
        i += w;
    }
}

// void NDEF_WriteSingleRecord(float temp, float rh)
// {
//     uint8_t buf[32];
//     uint16_t i = 0;

//     int t = (int)(temp * 10);
//     int h = (int)(rh * 10);

//     // ---- TLV ----
//     buf[i++] = 0x03;      // TLV tag

//     uint16_t len_index = i++;
    
//     // ---- NDEF Header ----
//     buf[i++] = 0xD1;      // MB=1 ME=1 SR=1 TNF=1
//     buf[i++] = 0x01;      // type length
//     buf[i++] = 0x00;      // payload length (update sau)
//     buf[i++] = 0x54;      // 'T'
//     buf[i++] = 0x02;      // lang length
//     buf[i++] = 'e';
//     buf[i++] = 'n';

//     // ---- TEXT ----
//     buf[i++] = (t/100)%10 + '0';
//     buf[i++] = (t/10)%10 + '0';
//     buf[i++] = '.';
//     buf[i++] = t%10 + '0';
//     buf[i++] = 'C';
//     buf[i++] = ' ';
//     buf[i++] = (h/100)%10 + '0';
//     buf[i++] = (h/10)%10 + '0';
//     buf[i++] = '.';
//     buf[i++] = h%10 + '0';
//     buf[i++] = '%';

//     uint8_t payload_len = 2 + 12; // "en" + text 11 char
//     buf[4] = payload_len;         // payload length
//     buf[len_index] = 4 + payload_len; // TLV length

//     buf[i++] = 0xFE;              // terminator

//     uint16_t addr = 0x0010;
//     uint16_t index = 0;
//     uint16_t len = sizeof(buf);

//     while (index < len)
//     {
//         uint8_t w = (len - index >= 3) ? 3 : (len - index);

//         FM13DT160_PowerOn();
//         FM13DT160_WriteEEPROM(addr, &buf[index], w);
//         FM13DT160_PowerOff();
//         delay_cycles(2400000);

//         addr += w;
//         index += w;
//     }
// }


float temp, rh;

int main(void)
{
    SYSCFG_DL_init();
    SEGGER_RTT_Init();
    FM13DT160_PowerOff();
    HDC302X_Init();
    SC7A20H_Init();

    uint16_t hdc_id = HDC302X_ReadManufacturerID();     // ID = 0x3000
    uint8_t  acc_id = SC7A20H_ReadWhoAmI();             // ID = 0x11

    // fm13_write_hello();
    // NDEF_Template_Init();
    // NDEF_Flush_All();

    FM13DT160_PowerOn();
    FM13DT160_ReadEEPROM(0x0000, verify_buf, 32);
    FM13DT160_PowerOff();
    delay_cycles(2400000);

    SC7A20H_EnableMotionInt();
    NVIC_EnableIRQ(GROUP0_INT_IRQN);
    src = SC7A20H_ReadIntSource();

    while (1)
    {
        HDC302X_ReadTempRH(&temp, &rh);

        if (temp != 0 && temp != -45)
        {
            //NDEF_WriteSingleRecord(temp, rh);
            // NDEF_Update_Record(current_record, temp, rh);
            // NDEF_Flush_All();

            // current_record++;
            // if (current_record >= NREC)
            //     current_record = 0;
        }

        if (gAccIntFlag) {
            gAccIntFlag = false;
            SC7A20H_ReadIntSource(); // clear INT
            SC7A20H_ReadXYZ(&ax, &ay, &az);
        }

        delay_cycles(50000000);
    }
}


void GPIOA_IRQHandler(void)
{
    uint32_t status;

    status = DL_GPIO_getEnabledInterruptStatus(
                GROUP0_PORT,
                GROUP0_ACC_INT1_PIN |
                GROUP0_ACC_INT2_PIN |
                GROUP0_TEMP_ALR_PIN |
                GROUP0_NFC_INT_PIN
            );

    DL_GPIO_clearInterruptStatus(GROUP0_PORT, status);

    if (status & GROUP0_ACC_INT1_PIN) {
        //gAccIntFlag = true;
    }

    if (status & GROUP0_ACC_INT2_PIN) {
        //gAccIntFlag = true;
    }

    if (status & GROUP0_TEMP_ALR_PIN) {
        //gTempIntFlag = true;
    }

    if (status & GROUP0_NFC_INT_PIN) {
        //gNfcIntFlag = true;
    }
}

void I2C_INST_IRQHandler(void)
{

}
