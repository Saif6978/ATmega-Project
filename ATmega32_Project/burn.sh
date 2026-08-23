#!/bin/bash

# ATmega32 burner script

MCU="atmega32"
PROGRAMMER="usbasp"

if [ -z "$1" ]; then
    echo "Usage: ./burn.sh filename"
    echo "Example: ./burn.sh blink"
    exit 1
fi

NAME=$1

SOURCE="codes/$NAME.c"
BUILD="build/$NAME"

if [ ! -f "$SOURCE" ]; then
    echo "Error: $SOURCE not found"
    exit 1
fi


echo "Compiling $SOURCE ..."

avr-gcc \
-mmcu=$MCU \
-Os \
$SOURCE \
-o $BUILD.elf


if [ $? -ne 0 ]; then
    echo "Compilation failed"
    exit 1
fi


echo "Creating HEX file..."

avr-objcopy \
-O ihex \
-R .eeprom \
$BUILD.elf \
$BUILD.hex


echo "Burning ATmega32..."

sudo avrdude \
-c $PROGRAMMER \
-p $MCU \
-U flash:w:$BUILD.hex


if [ $? -eq 0 ]; then
    echo "========================"
    echo " SUCCESS: ATmega32 flashed "
    echo "========================"
else
    echo "Burning failed"
fi