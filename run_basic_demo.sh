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

./build_bins.sh $1
$X16EMU -bas vga.bas -run

