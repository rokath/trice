# Makefile for compiling the Trice demo on LibOpenCM3
# for STM32F411-Nucleo boards
CC=arm-none-eabi-gcc
C_FLAGS=-Os -std=c99 -ggdb3
C_FLAGS+=-mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
C_FLAGS+=-Wextra -Wshadow -Wimplicit-function-declaration -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
C_FLAGS+=-fno-common -ffunction-sections -fdata-sections  -MD -Wall -Wundef
C_FLAGS+=-DSTM32F4 -I/home/kraiskil/stuff/libopencm3/include
# These two are for trice.h and triceConfig.h
C_FLAGS+=-I../../pkg/src/ -I.

LDFLAGS=-L${OPENCM3_DIR}/lib -lopencm3_stm32f4 -lm -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
LDFLAGS+=-T nucleo-f411re.ld
LDFLAGS+=--static -nostartfiles

all: demo.elf
.PHONY: flash clean

# Annotate Trice-enabled code.
# trice does this annotation in-place, so here we take
# a copy before running trice.
# I.e. write TRICE macros in foo.c, and this will generate
# the TRICE( Id(1234) .. ) macros into foo.trice.c
%.trice.c: %.c til.json
	cp -f $< $<.bak
	trice update
	cp -f $< $@
	cp -f $<.bak $<

# trice expects this file to exist, can be empty.
til.json:
	touch til.json

demo.elf: main.trice.c time.c ../../pkg/src/trice.c
	${CC} ${C_FLAGS} $^ -o $@ ${LDFLAGS}

flash: demo.elf
	openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c "program demo.elf verify reset exit"

clean:
	rm -f demo.elf til.json main.trice.c
