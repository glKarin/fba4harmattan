#!/bin/sh



HARM="--harm-layout=0 --harm-debug=0 --harm-joystick-mode=1 --harm-BIOS="
GDB="gdb --args "

${GDB2} ./built/fba --clock=600 --sense=100 --sound-sdl --samplerate=11025 --scaling=3 --rotate=0 --z80core=0 --filter=0 ${HARM}'AES Asia' --harm-DIP='' --harm-show-2p $*
