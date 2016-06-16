# makfile configuration
NAME            = mcu_firmware
SRC				= $(wildcard src/*.c)
LIB             = $(wildcard lib/**/*.c)
LIB             += $(wildcard lib/*/*/*.c)
OBJ             = $(SRC:.c=.o) $(LIB:.c=.o)

CFLAGS          = -O2 -Ilib -Wall -g -mcpu=430 -mivcnt=16 -mmpy=16      # Uniarch flags

# use custom linker script
LDFLAGS         = -Tlink.ld -Ilib

#switch the compiler (for the internal make rules)
CC              = msp430-gcc
LD              = msp430-gcc

TOOL_PATH = $(LIBSOFTCORE_PATH)/opencores/openmsp430/tools/bin

.PHONY: all FORCE clean download download-jtag download-bsl dist

#all should be the first target. it's built when make is runwithout args
all: ${NAME}.elf $(NAME).bin
#all: ${NAME}.elf $(NAME).bin ${NAME}.a43 ${NAME}.lst $(NAME).mif ../bin/$(NAME).bin

#confgigure the next line if you want to use the serial download
download: download-uart
#download: download-jtag
#download: download-bsl

#additional rules for files

#link everything together
${NAME}.elf: ${OBJ}
	${LD} $(LDFLAGS) -o $@ ${OBJ}

# binary image, then .MIF file for quartus
${NAME}.bin: ${NAME}.elf
	msp430-objcopy -O binary --gap-fill 255 $^ $@
../bin/$(NAME).bin: $(NAME).bin
	cp $^ $@
$(NAME).mif: $(NAME).bin
#	../bin/mifwrite $^ $@
#	../../tools/bin/mifwrite $^ $@
	@echo "mifwrite not available for the moment."

# ihex image
${NAME}.a43: ${NAME}.elf
	msp430-objcopy -O ihex $^ $@

# disassembler file
${NAME}.lst: ${NAME}.elf
	msp430-objdump -dSt $^ >$@

download-jtag: all
	msp430-jtag -e ${NAME}.elf

download-bsl: all
	msp430-bsl -e ${NAME}.elf

download-uart: all
	${TOOL_PATH}/openmsp430-loader.tcl -device /dev/ttyUSB0 -baudrate 115200 ${NAME}.elf

bin:
	./tools/init_ram_pkg/init_ram_pkg mcu_firmware.bin > init_ram16_pkg.vhd
	cp init_ram16_pkg.vhd $(LIBSOFTCORE_PATH)/src/omsp_peripherals/ram16/rtl/

clean:
	rm -f ${NAME} ${NAME}.a43 ${NAME}.lst $(NAME).bin $(NAME).mif $(NAME).elf ${OBJ}

#backup archive
dist:
	tar czf dist.tgz *.c *.h *.txt Makefile

#dummy target as dependecy if something has to be build everytime
FORCE:

#project dependencies
main.o: main.c 
