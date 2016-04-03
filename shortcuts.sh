#!/usr/bin/env bash

alias sjs-docker-fix-network='sudo route -nv add -net 192.168.99 -interface vboxnet0'
alias sjs-build-and-flash-latest='pushd ~/iot-src/smart.js/smartjs/platforms/esp8266; make; /Applications/FNC.app/Contents/MacOS/FNC --flash-baud-rate 115200 --flash firmware/smartjs-esp8266-last.zip --platform esp8266 --port /dev/cu.wchusbserial1410; popd'
alias sjs-flash-latest='pushd ~/iot-src/smart.js/smartjs/platforms/esp8266; /Applications/FNC.app/Contents/MacOS/FNC --flash-baud-rate 115200 --flash firmware/smartjs-esp8266-last.zip --platform esp8266 --port /dev/cu.wchusbserial1410; popd'
alias sjs-clean='pushd ~/iot-src/smart.js/smartjs/platforms/esp8266; make clean; popd'
