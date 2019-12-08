#!/bin/bash
if [ -z "${X16EMU}" ]
then
   if [ -z "${X16EMU_HOME}" ]
   then
      X16EMU=../x16-emulator/x16emu
   else
      X16EMU=${X16EMU_HOME}/x16emu
   fi
else
   X16EMU=${X16EMU}
fi

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

rm *.BIN
./convbin.exe $1 BITMAP.B000.BIN A000
./pal12bit.exe $pal PAL.B010.BIN A000
./colbanks.exe
cl65 --cpu 65C02 -o bankedvga.prg -l bankedvga.list bankedvga.asm
$X16EMU -prg bankedvga.prg -run
