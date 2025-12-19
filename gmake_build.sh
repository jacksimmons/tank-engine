#!/bin/sh
premake/premake5 gmake

# First param = Clean
if [ $# -ne 0 ]; then
    make clean
fi
make