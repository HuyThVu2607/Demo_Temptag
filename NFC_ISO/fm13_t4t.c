#include "fm13_t4t.h"
#include "Driver/fm13dt160.h"
#include <string.h>
#include <stdbool.h>
#include "ti_msp_dl_config.h"

static void fm13_write_bytes(uint16_t addr, const uint8_t* data, uint16_t len) {
    uint16_t off = 0;

    while(off < len) {
        uint8_t chunk = (uint8_t)((len - off) >= 3u ? 3u : (len - off));
        FM13DT160_WriteEEPROM(addr + off, (uint8_t*)&data[off], chunk);
        off += chunk;
    }
}

void fm13_t4t_init_cc(void) {
    // Minimal Type-4 CC file (15 bytes)
    // CCLEN 0x000F
    // Version 0x20 (2.0)
    // MLe 0x00FF, MLc 0x00FF
    // NDEF File Control TLV:
    //   T=0x04, L=0x06,
    //   FileID=E104,
    //   MaxSize=00FF,
    //   Read=00, Write=00 (read/write allowed)
    const uint8_t cc[15] = {
        0x00, 0x0F,
        0x20,
        0x00, 0xFF,
        0x00, 0xFF,
        0x04, 0x06,
        0xE1, 0x04,
        0x00, 0xFF,
        0x00,
        0x00
    };

    FM13DT160_PowerOn();
    fm13_write_bytes(FM13_T4T_CC_ADDR, cc, (uint16_t)sizeof(cc));
    FM13DT160_PowerOff();
    delay_cycles(2400000);
}

void fm13_t4t_write_ndef(const uint8_t* ndef, uint16_t ndef_len) {
    if(!ndef) return;
    if(ndef_len > FM13_T4T_NDEF_MAX) ndef_len = FM13_T4T_NDEF_MAX;

    uint8_t nlen[2];
    nlen[0] = (uint8_t)(ndef_len >> 8);
    nlen[1] = (uint8_t)(ndef_len & 0xFF);

    FM13DT160_PowerOn();

    // Write NLEN first
    fm13_write_bytes(FM13_T4T_NDEF_ADDR, nlen, 2);

    // Then payload
    fm13_write_bytes((uint16_t)(FM13_T4T_NDEF_ADDR + 2u), ndef, ndef_len);

    FM13DT160_PowerOff();
    delay_cycles(2400000);
}

uint16_t ndef_build_text_record(uint8_t* out_ndef, uint16_t cap, const char* text) {
    if(!out_ndef || !text) return 0;

    const uint16_t tlen = (uint16_t)strlen(text);
    const uint16_t need = (uint16_t)(7u + tlen); // D1 01 PLEN 54 02 'e' 'n' + text

    if(cap < need) return 0;

    uint16_t i = 0;
    out_ndef[i++] = 0xD1;                 // MB=1, ME=1, SR=1, TNF=0x01 (well-known)
    out_ndef[i++] = 0x01;                 // type length = 1 ('T')
    out_ndef[i++] = (uint8_t)(tlen + 3u); // payload length
    out_ndef[i++] = 0x54;                 // 'T'
    out_ndef[i++] = 0x02;                 // status: UTF-8 + lang length=2
    out_ndef[i++] = 'e';
    out_ndef[i++] = 'n';

    memcpy(&out_ndef[i], text, tlen);
    i = (uint16_t)(i + tlen);

    return i; // record length
}
