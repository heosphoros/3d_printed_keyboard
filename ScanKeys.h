#ifndef _SCAN_KEYS_H_
#define _SCAN_KEYS_H_

#include <stdint.h>
#include <LUFA/Drivers/Peripheral/TWI.h>

struct Key {
  uint8_t row;
  uint8_t column;
  uint8_t state;
  uint8_t just_pressed;
  uint8_t just_released;
};

extern uint8_t ScanKeys_Right_Side_Disconnected;

void ScanKeys_Init(void);
void ScanKeys_Read(void (*scan_keys_callback)(struct Key, void *data), void *data);

#define SCAN_MATRIX_ROWS 7
#define SCAN_MATRIX_COLUMNS 16

#endif
