#!/bin/bash

# When making a redistributable linux installation, use this
# to check the required librearies after it's installed on
# another machine

ldd gcc/arm-miosix-eabi/bin/* | perl -e 'while(<>){next unless(/\s+(\S+.so(\S+))\s+/);print "$1\n";}' | sort -u
