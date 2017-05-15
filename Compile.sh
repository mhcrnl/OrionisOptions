#!/bin/bash

# Compile and execute Orionis Options
# NOTE: This script must be in the root directory of the Sources folder (i.e. ../Sources)

gcc -o OrionisOptions -Wall Sources/*.c `pkg-config --libs --cflags gtk+-3.0` && ./OrionisOptions && clear
