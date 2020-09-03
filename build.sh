#!/bin/bash

WD=$(pwd)

if mkdir -p build && cd $WD/build && cmake .. && make all && cd $WD; then
    echo "BUILD SUCCESS"
else
    echo "BUILD FAILURE"
    cd $WD
fi
