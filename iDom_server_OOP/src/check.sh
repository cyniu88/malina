#!/bin/sh
cppcheck --enable=all --inconclusive --std=c++11 . 2> err.txt
