/**
  ******************************************************************************
  * @file           : cartridge_hal.cpp
  * @brief          : Implementation of VCS game cartridge hardware abstraction
  ******************************************************************************
  */



//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <Arduino.h>
#include <stdbool.h>
#include "cartridge_hal.h"
#include "system.h"


//------------------------------------------------------------------------------
// Private function prototypes
//------------------------------------------------------------------------------
static void DataBusMode(WiringPinMode mode);


//------------------------------------------------------------------------------
// Module data
//------------------------------------------------------------------------------
static const uint16_t kWriteProtected = 0x1000; /**< Address of ROM (game cartridge) */
static bool driveDataBus = false; /**< Data direction shadow register */
static uint8_t dataBus = 0; /**< Data bus shadow register */
static uint16_t addressBus = 0; /**< Address bus shadow register */


//------------------------------------------------------------------------------
// Public functions - Init
//------------------------------------------------------------------------------
void HAL_Cartridge_Init(void) {
  pinMode(PA15, OUTPUT);
  pinMode(PB3, OUTPUT);
  pinMode(PB4, OUTPUT);
  pinMode(PB5, OUTPUT);
  pinMode(PB6, OUTPUT);
  pinMode(PB7, OUTPUT);
  pinMode(PB8, OUTPUT);
  pinMode(PB9, OUTPUT);
  pinMode(PA0, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PA3, OUTPUT);
  pinMode(PA2, OUTPUT);
  pinMode(PA4, OUTPUT);
}


//------------------------------------------------------------------------------
// Public functions
//------------------------------------------------------------------------------
/**
 * @brief Set the address bus value. Sets the databus to input for address in ROM. To output otherwise.
 * @param address[in]
 */
void HAL_Cartridge_SetAddressBus(uint16_t address)
{
  // Limit address to 6508 address space
  addressBus = address & ADDRESS_RANGE;

  // Disable ROM while address changes
  digitalWrite(PA4, LOW);

  if (addressBus < kWriteProtected)
  {
    HAL_Cartridge_DataBusOutput();
  }
  else
  {
    HAL_Cartridge_DataBusInput();
  }

  digitalWrite(PA15, (bool)(addressBus & (1 << 0)) );
  digitalWrite(PB3, (bool)(addressBus & (1 << 1)) );
  digitalWrite(PB4, (bool)(addressBus & (1 << 2)) );
  digitalWrite(PB5, (bool)(addressBus & (1 << 3)) );
  digitalWrite(PB6, (bool)(addressBus & (1 << 4)) );
  digitalWrite(PB7, (bool)(addressBus & (1 << 5)) );
  digitalWrite(PB8, (bool)(addressBus & (1 << 6)) );
  digitalWrite(PB9, (bool)(addressBus & (1 << 7)) );
  digitalWrite(PA0, (bool)(addressBus & (1 << 8)) );
  digitalWrite(PA1, (bool)(addressBus & (1 << 9)) );
  digitalWrite(PA3, (bool)(addressBus & (1 << 10)) );
  digitalWrite(PA2, (bool)(addressBus & (1 << 11)) );
  digitalWrite(PA4, (bool)(addressBus & (1 << 12)) );
}


void HAL_Cartridge_SetDataBus(uint8_t data)
{
  dataBus = data;

  digitalWrite(PA8, (dataBus & (1 << 0)));
  digitalWrite(PA9, (dataBus & (1 << 1)));
  digitalWrite(PA10, (dataBus & (1 << 2)));
  digitalWrite(PB11, (dataBus & (1 << 3)));
  digitalWrite(PB10, (dataBus & (1 << 4)));
  digitalWrite(PB13, (dataBus & (1 << 5)));
  digitalWrite(PB14, (dataBus & (1 << 6)));
  digitalWrite(PB15, (dataBus & (1 << 7)));
}


uint8_t HAL_Cartridge_GetDataBus(void)
{
  uint8_t data = 0;

  if (driveDataBus)
  {
    data = dataBus;
  }
  else
  {
    data =  (bool)digitalRead(PA8) ? (1 << 0) : 0;
    data |= (bool)digitalRead(PA9) ? (1 << 1) : 0;
    data |= (bool)digitalRead(PA10) ? (1 << 2) : 0;
    data |= (bool)digitalRead(PB11) ? (1 << 3) : 0;
    data |= (bool)digitalRead(PB10) ? (1 << 4) : 0;
    data |= (bool)digitalRead(PB13) ? (1 << 5) : 0;
    data |= (bool)digitalRead(PB14) ? (1 << 6) : 0;
    data |= (bool)digitalRead(PB15) ? (1 << 7) : 0;
  }

  return data;
}


/**
 * @brief Set data bus pins to input.
 */
void HAL_Cartridge_DataBusInput(void)
{
  driveDataBus = false;
  DataBusMode(INPUT_PULLUP);
}


/**
 * @brief Set data bus pins to output.
 */
void HAL_Cartridge_DataBusOutput(void)
{
  driveDataBus = true;
  DataBusMode(OUTPUT);
}


//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
static void DataBusMode(WiringPinMode mode)
{
  pinMode(PA8, mode);
  pinMode(PA9, mode);
  pinMode(PA10, mode);
  pinMode(PB11, mode);
  pinMode(PB10, mode);
  pinMode(PB13, mode);
  pinMode(PB14, mode);
  pinMode(PB15, mode);
}
