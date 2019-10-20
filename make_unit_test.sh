#!/bin/bash

echo "start build"

cd iDom*/test/iDom* || exit
ls
cmake .
make
./iDom_TESTs-CMAKE