#!/bin/sh
if [ -z $1 ]; then
  echo "Usage: ${0} <firmware.hex> [device]"
  exit 1
fi

if [ $2 ]; then
  PORT="${2}"
else
  PORT="/dev/ttyUSB0"
fi

stty -F "${PORT}" hupcl
avrdude -F -p m644p -c stk500v1 -P "${PORT}" -b 38400 -Uflash:w:"${1}"
