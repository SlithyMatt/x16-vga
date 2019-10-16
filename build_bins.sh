#!/bin/bash

if [ $# -lt 1 ]; then
   echo "Usage: $0 [raw image data file]"
   exit 1
fi

if [ -f $1 ]; then
   raw=$1
else
   echo "Error: $1 is not a file"
   exit 1
fi

pal="$raw.pal"

if [ ! -f $pal ]; then
   echo "Error: $pal is not a file"
   exit 1
fi

./convbin.exe $raw BITMAP.BIN 4000
./pal12bit.exe $pal PAL.BIN


