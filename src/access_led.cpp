/**
  ******************************************************************************
  * @file           : access_led.cpp
  * @brief          : Implementation of access led control
  ******************************************************************************
  */
#include <Arduino.h>
void AccessLed_Init(void)
{
    pinMode(PC13, OUTPUT);
}

/**
 * @brief Turn on Access LED
 */
void AccessLed_On(void)
{
  digitalWrite(PC13, LOW);
}

/**
 * @brief Turn off Access LED
 */
void AccessLed_Off(void)
{
  digitalWrite(PC13, HIGH);
}
