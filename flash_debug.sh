#!/bin/bash

openocd -f interface/stlink.cfg -f target/stm32g4x.cfg -c "program build/Debug/shield_raspberry_main.elf verify reset exit"
