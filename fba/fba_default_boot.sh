#!/bin/sh

# The arguments of "--scaling=3" and "--rotate=0" should be not changed!
# And other arguments like Maemo 5 version.
#  - by Karin Zhao (dospy - 香磷ヽ) 
echo "/usr/bin/fba --clock=600 --sense=100 --sound-sdl --samplerate=11025 --scaling=3 --rotate=0 --z80core=0 --filter=0 $1"
/usr/bin/fba --clock=600 --sense=100 --sound-sdl --samplerate=11025 --scaling=3 --rotate=0 --z80core=0 --filter=0 $1
exit 0
