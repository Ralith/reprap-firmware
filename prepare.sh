#!/bin/sh

if [ -z $1 ]; then
  echo "Usage: ${0} <ELF binary>"
  exit 1
fi

avr-objcopy -O ihex -R .eeprom "${1}" "${1}.hex"
