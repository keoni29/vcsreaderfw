
/**
  ******************************************************************************
  * @file           : main.cpp
  * @brief          : Atari VCS/ Atari 2600 video game cartridge reader firmware
  * @mainpage Overview
  * Usage:
  * -# Open serial port e.g. /dev/ttyACM0
  * -# Send request
  * -# Receive reply
  * -# (Optional) Unpack device info. See ::InfoTypedef 
  * 
  * Request/reply structure:
  * offset          | Field name
  * --------------- | ----------------------------
  * +0              | Header (See ::HeaderTypeDef )
  * +sizeof(Header) | Data
  ******************************************************************************
  */

//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include <Arduino.h>
#include "access_led.h"
#include "cartridge.h"
#include "system.h"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define BUFFER_SIZE 8192

//------------------------------------------------------------------------------
// Typedefs
//------------------------------------------------------------------------------
typedef enum{
  READ_SINGLE = 'r',    /**< Read from a single memory address */
  WRITE_SINGLE = 'w',   /**< Write to a single memory address TODO implement */
  EMULATE_SINGLE = 'e', /**< Emulate reading from a single memory address TODO implemenent */
  READ_BLOCK = 'R',     /**< Read a block of memory */
  WRITE_BLOCK = 'W',    /**< Write a block of memory TODO implement */
  EMULATE_BLOCK = 'E',  /**< Emulate reading from a block of memory TODO implement */
  SET_READ_DELAY = 'd', /**< Set the delay between reads in ms TODO implement */
  GET_READ_DELAY = 'D', /**< Get the delay between reads in ms TODO implement */
  GET_INFO = 'I',       /**< Get firmware/hardware version info */
  SYNC = 'S',           /**< Synchronizes soft and firmware by resetting the interpreter. Synchronization character, not an actual command. */
}CmdTypedef;


typedef enum{
  ERROR_COMMAND     = 1,  /**< Unknown command */
  ERROR_CHECKSUM    = 2,  /**< Wrong checksum */
  ERROR_LENGTH      = 4,  /**< Data length exceeds limit */
  ERROR_RANGE       = 8,  /**< Value out of range */
  ERROR_TIMEOUT     = 16, /**< Timeout **/
  ERROR_REPLY_LENGTH = 32 /**< BUG: Reply length exceeds buffer size.*/
}ErrorTypeDef;


typedef struct __attribute__((packed)){ /* Packed so structure is well defined. Required for communication with software domain. */
  uint8_t cmd;            /**< Command \n Available commands: ::CmdTypedef */
  uint8_t status;         /**< Status + errors field. Upper 4 bits are error flags.*/
  uint16_t requestLength; /**< Length of the request frame (from software) */
  uint16_t replyLength;   /**< Length of the reply frame (to software) */
  uint16_t address;       /**< Target address in 6508 address space */
  uint16_t checksum;      /**< Checksum must be last element in struct */
} HeaderTypeDef;


typedef struct __attribute__((packed)){
  uint32_t uniqueid;
  uint16_t devicetype;
  uint8_t hwversion;
  uint8_t hwrevision;
  uint8_t fwversion;
  uint8_t fwrevision;
} InfoTypedef;


//------------------------------------------------------------------------------
// Private function prototypes
//------------------------------------------------------------------------------
static uint16_t CalculateChecksum(uint8_t *buf, uint16_t len);


//------------------------------------------------------------------------------
// Private function prototypes
//------------------------------------------------------------------------------
uint8_t data[BUFFER_SIZE];
InfoTypedef *info = (InfoTypedef *)data;


//------------------------------------------------------------------------------
// Public function - Setup
//------------------------------------------------------------------------------
void setup()
{
  AccessLed_Init();
  Cartridge_Init();
}

//------------------------------------------------------------------------------
// Public function - Loop
//------------------------------------------------------------------------------
void loop(void)
{
  HeaderTypeDef header;
  uint16_t checksum;
  uint16_t errorFlags;
  size_t len;

  Serial.begin();
  // Wait for serial port to be connected.
  while (!Serial) {};

  for (int i = 0; i < 5; i++) {
    AccessLed_On();
    delay(200);
    AccessLed_Off();
    delay(100);
  }

  while (1)
  {
    errorFlags = 0;

    if (!Serial)
    {
      // Restart interpreter
      break;
    }

    len = Serial.readBytes((char *)&header, sizeof(header));
    if (header.cmd == SYNC || len == 0) {
      // Serial timed out or synchronizing, try again.
      continue;
    }

    if (len < sizeof(header)) {
      errorFlags |= ERROR_TIMEOUT;
    }

    if (!errorFlags)
    {
      if (header.requestLength > sizeof(data))
      {
        errorFlags |= ERROR_LENGTH;
      }
      else
      {
        len = Serial.readBytes((char *)data, header.requestLength);
        if (len < header.requestLength) {
          errorFlags |= ERROR_TIMEOUT;
        }

        checksum = CalculateChecksum((uint8_t *)&header, sizeof(header) - sizeof(header.checksum));
        checksum += CalculateChecksum(&data[0], header.requestLength);

        if (header.checksum != checksum)
        {
          errorFlags |= ERROR_CHECKSUM;
        }

        if (header.address > ADDRESS_RANGE)
        {
          errorFlags |= ERROR_RANGE;
        }
      }

      // Prevent buffer overflow of reply data.
      // TODO repeated cartridge reads instead of limiting replyLength
      if (header.replyLength > sizeof(data))
      {
        header.replyLength = sizeof(data);
      }
    }

    if (!errorFlags)
    {
      AccessLed_On();

      switch (header.cmd)
      {
      case READ_SINGLE:
        data[0] = Cartridge_Read(header.address);
        header.replyLength = 1;
        break;

      case WRITE_SINGLE: //TODO implement
        errorFlags |= ERROR_COMMAND;
        break;

      case EMULATE_SINGLE:
        Cartridge_ReadEmulated(header.address, data[0]);
        header.replyLength = 1;
        break;

      case READ_BLOCK:
        Cartridge_ReadBlock(header.address, header.replyLength, data);
        break;

      case WRITE_BLOCK: //TODO implement
        errorFlags |= ERROR_COMMAND;
        break;

      case EMULATE_BLOCK: //TODO implement
        errorFlags |= ERROR_COMMAND;
        break;

      case SET_READ_DELAY: //TODO implement
        errorFlags |= ERROR_COMMAND;
        break;

      case GET_READ_DELAY: //TODO implement
        errorFlags |= ERROR_COMMAND;
        break;

      case GET_INFO: // TODO move magic numbers into struct
        info->uniqueid = 0xEFBEADDE;
        info->devicetype = 0xEFBE;
        info->hwversion = 0x03;
        info->hwrevision = 0x00;
        info->fwversion = 0x03;
        info->fwrevision = 0x00;
        header.replyLength = sizeof(InfoTypedef);
        break;

      default:
        errorFlags |= ERROR_COMMAND;
        break;
      }
      
      AccessLed_Off();
    }

    if (header.replyLength > sizeof(data)) {
      errorFlags |= ERROR_REPLY_LENGTH;
    }

    if (errorFlags)
    {
      header.status = errorFlags;
      header.replyLength = 0;
    }

    header.checksum = CalculateChecksum((uint8_t *)&header, sizeof(header) - sizeof(header.checksum));
    header.checksum += CalculateChecksum(data, header.replyLength);

    Serial.write((uint8_t *)&header, sizeof(header));
    Serial.write(data, header.replyLength);
  }
}


//------------------------------------------------------------------------------
// Private functions
//------------------------------------------------------------------------------
static uint16_t CalculateChecksum(uint8_t *buf, uint16_t len)
{
  uint16_t checksum = 0;
  uint16_t i;

  for (i = 0; i < len; i++)
  {
    checksum += buf[i];
  }

  return checksum;
}
