## Building firmware
This firmware was built using platformIO for stm32. Board info can be found here:
- https://wiki.stm32duino.com/index.php?title=Blue_Pill

## Uploading firmware
Type without hitting enter
```
dfu-util -a2 -D .pioenvs/bluepill_f103c8/firmware.bin
```
Press the reset button and then press enter
