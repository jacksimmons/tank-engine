#!/bin/bash
premake/premake5 gmake

# Clean build
# Only generate compile_commands.json if a clean build was requested
if [[ "$1" = "--clean" || "$1" = "-c" ]]; then
    make clean
    bear -- make all CFLAGS="-fPIC"
fi

perl -i -0pe 's|("-include",\s*")[^"]*tepch\.hpp"|$1'"$(pwd)"'/TankEngine/tepch.hpp"|g' compile_commands.json