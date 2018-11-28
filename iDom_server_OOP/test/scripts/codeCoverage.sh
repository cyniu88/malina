#!/bin/bash
../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/iDom_TESTs-CMAKE
echo "generuje raport"
llvm-profdata-3.9  merge -o ../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/test.profdata ../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/default.profraw
echo "tworze html"

llvm-cov-3.9 show ../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/test.profdata  -path-equivalence -use-color --format html > /tmp/coverage.html
echo "wysylam html na server"
scp -P 11 /tmp/coverage.html  root@192.168.1.1:/www_php/coverage.html
echo "koniec"
llvm-cov-3.9  report ../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/test.profdata -use-color | sed -e 's/\/home\/cyniu\/GIT\/malina\/iDom_server_OOP\///g' 

llvm-cov-3.9 show ../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/test.profdata  -path-equivalence  --format text > coverage.txt
llvm-cov-3.9 report ../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Desktop_Qt_5_11_2_GCC_64bit-Debug/test.profdata  > report.txt

sed -i 's/\/home\/cyniu\/GIT\/malina\/iDom_server_OOP\///g' report.txt

cp /tmp/coverage.html  ./coverage.html
