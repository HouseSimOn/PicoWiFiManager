#!/bin/zsh

print "Program and Verify"
picotool load -v -x build/WifiManager.uf2 -f

sleep 2
print "Start screen"
screen /dev/tty.usbmodem101 115200