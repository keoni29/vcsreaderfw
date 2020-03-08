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
#include "cartridge_wiring.h"
#include "system.h"


//------------------------------------------------------------------------------
// Private function prototypes
//------------------------------------------------------------------------------
static void DataBusMode(uint32_t mode);


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
  pinMode(A0_PIN, OUTPUT);
  pinMode(A1_PIN, OUTPUT);
  pinMode(A2_PIN, OUTPUT);
  pinMode(A3_PIN, OUTPUT);
  pinMode(A4_PIN, OUTPUT);
  pinMode(A5_PIN, OUTPUT);
  pinMode(A6_PIN, OUTPUT);
  pinMode(A7_PIN, OUTPUT);
  pinMode(A8_PIN, OUTPUT);
  pinMode(A9_PIN, OUTPUT);
  pinMode(A10_PIN, OUTPUT);
  pinMode(A11_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);

  HAL_Cartridge_DataBusInput();
  HAL_Cartridge_DisableRom();
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

  if (addressBus < kWriteProtected)
  {
    HAL_Cartridge_DataBusOutput();
  }
  else
  {
    HAL_Cartridge_DataBusInput();
  }

  digitalWrite(A0_PIN, (bool)(addressBus & (1 << 0)) );
  digitalWrite(A1_PIN, (bool)(addressBus & (1 << 1)) );
  digitalWrite(A2_PIN, (bool)(addressBus & (1 << 2)) );
  digitalWrite(A3_PIN, (bool)(addressBus & (1 << 3)) );
  digitalWrite(A4_PIN, (bool)(addressBus & (1 << 4)) );
  digitalWrite(A5_PIN, (bool)(addressBus & (1 << 5)) );
  digitalWrite(A6_PIN, (bool)(addressBus & (1 << 6)) );
  digitalWrite(A7_PIN, (bool)(addressBus & (1 << 7)) );
  digitalWrite(A8_PIN, (bool)(addressBus & (1 << 8)) );
  digitalWrite(A9_PIN, (bool)(addressBus & (1 << 9)) );
  digitalWrite(A10_PIN, (bool)(addressBus & (1 << 10)) );
  digitalWrite(A11_PIN, (bool)(addressBus & (1 << 11)) );
}


void HAL_Cartridge_SetDataBus(uint8_t data)
{
  dataBus = data;

  digitalWrite(D0_PIN, (dataBus & (1 << 0)));
  digitalWrite(D1_PIN, (dataBus & (1 << 1)));
  digitalWrite(D2_PIN, (dataBus & (1 << 2)));
  digitalWrite(D3_PIN, (dataBus & (1 << 3)));
  digitalWrite(D4_PIN, (dataBus & (1 << 4)));
  digitalWrite(D5_PIN, (dataBus & (1 << 5)));
  digitalWrite(D6_PIN, (dataBus & (1 << 6)));
  digitalWrite(D7_PIN, (dataBus & (1 << 7)));
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
    data =  (bool)digitalRead(D0_PIN) ? (1 << 0) : 0;
    data |= (bool)digitalRead(D1_PIN) ? (1 << 1) : 0;
    data |= (bool)digitalRead(D2_PIN) ? (1 << 2) : 0;
    data |= (bool)digitalRead(D3_PIN) ? (1 << 3) : 0;
    data |= (bool)digitalRead(D4_PIN) ? (1 << 4) : 0;
    data |= (bool)digitalRead(D5_PIN) ? (1 << 5) : 0;
    data |= (bool)digitalRead(D6_PIN) ? (1 << 6) : 0;
    data |= (bool)digitalRead(D7_PIN) ? (1 << 7) : 0;
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

/**
 * @brief Enable cartridge ROM output if not driving the output pins.
 */
void HAL_Cartridge_EnableRom(void) {
  if (!driveDataBus) {
    digitalWrite(CS_PIN, HIGH);
  } else {
    // Safeguard to prevent damage to cartridge.
    // TODO raise error for this condition
    HAL_Cartridge_DisableRom();
  }
}


/**
 * @brief Disable cartridge ROM output.
 */
void HAL_Cartridge_DisableRom(void) {
  digitalWrite(CS_PIN, LOW);
}

//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
static void DataBusMode(uint32_t mode)
{
  pinMode(D0_PIN, mode);
  pinMode(D1_PIN, mode);
  pinMode(D2_PIN, mode);
  pinMode(D3_PIN, mode);
  pinMode(D4_PIN, mode);
  pinMode(D5_PIN, mode);
  pinMode(D6_PIN, mode);
  pinMode(D7_PIN, mode);
}
