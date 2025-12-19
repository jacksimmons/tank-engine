#!/bin/sh
premake/premake5 gmake

# First param = Clean
if [ $# -ne 0 ]; then
    gmake clean
fi
make