
/**
  ******************************************************************************
  * @file           : cartridge_wiring.h
  * @brief          : Cartridge wiring definitions
  * 
  * Usage:
  * Define the pin number for each cartridge connection
  ******************************************************************************
  */

#ifndef CARTRIDGE_WIRING_H
#define CARTRIDGE_WIRING_H

#define A0_PIN PA15
#define A1_PIN PB3
#define A2_PIN PB4
#define A3_PIN PB5
#define A4_PIN PB6
#define A5_PIN PB7
#define A6_PIN PB8
#define A7_PIN PB9
#define A8_PIN PA0
#define A9_PIN PA1
#define A10_PIN PA3
#define A11_PIN PA2
#define A12_PIN PA4
#define CS_PIN A12_PIN /* The A12 pin is used as a chipselect line for the cartridge ROM. */
#define D0_PIN PA8
#define D1_PIN PA9
#define D2_PIN PA10
#define D3_PIN PB11
#define D4_PIN PB10
#define D5_PIN PB13
#define D6_PIN PB14
#define D7_PIN PB15

#endif /* CARTRIDGE_WIRING_H */

