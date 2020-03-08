REM Upload script for sending the binary file to the Bluepill board.
dfu-util -a2 -D .pio/build/bluepill_f103c8/firmware.bin