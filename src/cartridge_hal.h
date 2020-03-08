
/**
  ******************************************************************************
  * @file           : cartridge_hal.h
  * @brief          : Header of VCS game cartridge hardware abstraction
  ******************************************************************************
  */

#ifndef CARTRIDGE_HAL_H_
#define CARTRIDGE_HAL_H_

void HAL_Cartridge_Init(void);
void HAL_Cartridge_SetAddressBus(uint16_t address);
void HAL_Cartridge_SetDataBus(uint8_t data);
uint8_t HAL_Cartridge_GetDataBus(void);
void HAL_Cartridge_DataBusInput(void);
void HAL_Cartridge_DataBusOutput(void);
void HAL_Cartridge_EnableRom(void);
void HAL_Cartridge_DisableRom(void);

#endif /* CARTRIDGE_HAL_H_ */
