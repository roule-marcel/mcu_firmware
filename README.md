# mcu_firmware
Robotics stuff firmware for the openMSP430-based mcu

## Install gcc-msp430
On ubuntu 15.04 (should also do on other versions):

    sudo apt-get install gcc-msp430

## Find TI's msp430 documentation
[TI's MSP430 datasheet](http://www.ti.com/lit/ug/slau049f/slau049f.pdf)

## HOWTO flash a MSP-EXP430F5529LP program

Use TI's MSPFlasher command-line tool (http://www.ti.com/tool/msp430-flasher)
* Ensure your compiled object is in Intel Hex (ihex) format. If needed, use objcopy for conversion
````(bash)
msp430-objcopy -O ihex "myprog.out" "myprog.hex"
````
* Then write it to the device (mspflasher will automatically look for a connected FET on /dev/ttyACM*)
````(bash)
mspflasher -w "myprog.hex" -v -g -z [VCC]
````
