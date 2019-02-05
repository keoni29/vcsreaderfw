
/**
  ******************************************************************************
  * @file           : cartridge.cpp
  * @brief          : Implementation of VCS game cartridge communication driver
  ******************************************************************************
  */

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "cartridge.h"
#include "cartridge_hal.h"

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Exported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private variables
//-----------------------------------------------------------------------------
static uint16_t readDelay = 0;  /**< Delay between reads. \n Unit: ms */
static const uint16_t kResetVector = 0xFFFC;  /**< Address of 6502 reset vector. */


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
void Cartridge_Init(void)
{
  HAL_Cartridge_Init();
  HAL_Cartridge_SetAddressBus(kResetVector);
}

/**
 * @brief Read from cartridge.
 * @param[in] address   Address in VCS memory address space
 * @return Byte read from the cartridge ROM.
 */
uint8_t Cartridge_Read(uint16_t address)
{
  uint8_t val;

  HAL_Cartridge_SetAddressBus(address);
  val = HAL_Cartridge_GetDataBus();

  return val;
}

/**
 * @brief Emulate read operation outside of ROM.
 * @param[in] address   Address in VCS memory address space
 * @return Byte read from the cartridge ROM.
 */
uint8_t Cartridge_ReadEmulated(uint16_t address, uint8_t data)
{
  HAL_Cartridge_SetAddressBus(address);
  HAL_Cartridge_SetDataBus(data);
  return CARTRIDGE_OK;
}

uint8_t Cartridge_ReadBlock(uint16_t start, uint16_t len, uint8_t* buf)
{
  uint8_t *p = &buf[0];
  uint32_t end = start + len;
  uint8_t val = 0;

  if (end < kResetVector)
  {
    for (uint32_t address = start; address < end; address++)
    {
      val = Cartridge_Read(address);
      *p = val;
      p++;
      //TODO put in delay
      //delay(readDelay);
    }

    return CARTRIDGE_OK;
  }
  else
  {
    return CARTRIDGE_RANGE;
  }
}

uint8_t Cartridge_ReadEmulatedBlock(uint16_t start, uint16_t len, uint8_t* buf)
{
  uint8_t *p = &buf[0];

  for (uint16_t address = start; address < address + len; address++)
  {
    Cartridge_ReadEmulated(address, *p);
    p++;
    delay(readDelay);
  }

  return CARTRIDGE_OK;
}

uint8_t Cartridge_SetReadDelay(uint16_t us)
{
  readDelay = us;
  return CARTRIDGE_OK;
}

/**
 * @brief Detect if a cartridge has been inserted.
 *
 * @return true when a cartridge is detected, false otherwise
 */
bool Cartridge_Detect(void)
{
  //TODO implement
  return true;
}
