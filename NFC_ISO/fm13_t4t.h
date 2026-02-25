#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// ===== FM13DT160 Type-4 Tag (T4T) minimal support =====
// CC file at 0xE103 (common for Type-4 tags)
// NDEF file at 0xE140 (common mapping)

#define FM13_T4T_CC_ADDR        (0xE103u)
#define FM13_T4T_NDEF_ADDR      (0xE140u)   // 2-byte NLEN + NDEF payload
#define FM13_T4T_NDEF_MAX       (0x00FFu)   // 255 bytes payload in this example

// Init CC file (call once at boot)
void fm13_t4t_init_cc(void);

// Write NDEF payload (record bytes). Function writes NLEN + payload.
void fm13_t4t_write_ndef(const uint8_t* ndef, uint16_t ndef_len);

// Helper build 1 Text record (UTF-8, "en") into out buffer.
// Return payload length written to out_ndef (record bytes only).
uint16_t ndef_build_text_record(uint8_t* out_ndef, uint16_t cap, const char* text);

#ifdef __cplusplus
}
#endif
