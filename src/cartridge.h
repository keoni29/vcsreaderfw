
/**
  ******************************************************************************
  * @file           : cartridge.h
  * @brief          : Header of VCS game cartridge communication driver
  ******************************************************************************
  */

#ifndef CARTRIDGE_H_
#define CARTRIDGE_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
  CARTRIDGE_OK = 0,
  CARTRIDGE_RANGE,
  CARTRIDGE_FAIL,
}Cartridge_StatusTypeDef;

void Cartridge_Init(void);
uint8_t Cartridge_Read(uint16_t address);
uint8_t Cartridge_ReadEmulated(uint16_t address, uint8_t data);
uint8_t Cartridge_ReadBlock(uint16_t start, uint16_t len, uint8_t* buf);
uint8_t Cartridge_ReadEmulatedBlock(uint16_t start, uint16_t len, uint8_t* buf);
uint8_t Cartridge_SetReadDelay(uint16_t us);
bool Cartridge_Detect(void);

#endif /* CARTRIDGE_H_ */
