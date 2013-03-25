#!/bin/sh

# Launcher in case dependencies are not avaible on the user system
# If dependencies are already installed, no need to use this script

LIBS_DIR=./lib32
EXECUTABLE=./wallbreaker

LD_LIBRARY_PATH="$LIBS_DIR":"$LD_LIBRARY_PATH" "$EXECUTABLE"

