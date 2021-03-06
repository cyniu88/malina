#!/bin/bash
../build-iDom_TESTs-CMAKE-clang-Debug/iDom_TESTs-CMAKE
echo "generuje raport"
llvm-profdata-3.9  merge -o ../build-iDom_TESTs-CMAKE-clang-Debug/test.profdata ../build-iDom_TESTs-CMAKE-clang-Debug/default.profraw
echo "tworze html"

llvm-cov-3.9 show ../build-iDom_TESTs-CMAKE-clang-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-clang-Debug/test.profdata  -filename-equivalence -use-color --format html > /tmp/coverage.html
echo "wysylam html na server"
scp -P 11 /tmp/coverage.html  root@192.168.1.1:/www_php/coverage.html
echo "koniec"
llvm-cov-3.9  report ../build-iDom_TESTs-CMAKE-clang-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-clang-Debug/test.profdata -use-color | sed -e 's/\/home\/pi\/programowanie\/iDom_server_OOP\///g'  | sed -e 's/test\/iDom_TESTs-CMAKE\/..\/..\///g' 

llvm-cov-3.9 show ../build-iDom_TESTs-CMAKE-clang-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-clang-Debug/test.profdata  -filename-equivalence  --format text > coverage.txt
llvm-cov-3.9 report ../build-iDom_TESTs-CMAKE-clang-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-clang-Debug/test.profdata  > report.txt

sed -i 's/\/home\/pi\/programowanie\/iDom_server_OOP\///g' report.txt

cp /tmp/coverage.html  ./coverage.html
