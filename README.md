# VCS/Atari2600 Game Cartridge Reader Firmware
This repository contains the firmware for a cartridge reader which I developed. It can be used to create backups of your atari2600 games on your PC for preservation purposes or for playing your games on an emulator.
I also made a piece of software for creating backups on your PC, which can be found at https://github.com/keoni29/romclient/
Visit my project page at https://hackaday.io/project/113217-atari2600-cartridge-reader

## Flashing the USB bootloader
Optional step if you wish to use the USB port to update device firmware.
The STM32 "Blue pill" board comes with a serial bootloader.
Use the serial bootloader to flash the USB bootloader.
Further instructions can be found here: https://revspace.nl/STM32

## Building firmware
This firmware was built using platformIO for stm32. Board info can be found here:
- https://wiki.stm32duino.com/index.php?title=Blue_Pill

## Uploading firmware
Using the USB bootloader

Type without hitting enter
```
dfu-util -a2 -D .pioenvs/bluepill_f103c8/firmware.bin
```
Press the reset button and then press enter
