#!/bin/bash

../build-iDom_TESTs-CMAKE-clang-Debug/iDom_TESTs-CMAKE
echo "generuje raport"
llvm-profdata-3.9  merge -o ../build-iDom_TESTs-CMAKE-clang-Debug/test.profdata ../build-iDom_TESTs-CMAKE-clang-Debug/default.profraw
echo "tworze html"

llvm-cov-3.9 show ../build-iDom_TESTs-CMAKE-clang-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-clang-Debug/test.profdata  -filename-equivalence -use-color --format html > /tmp/coverage.html
echo "wysylam html na server"
#wput  -d  /tmp/coverage.html  ftp://$USER:$PASS@$HOST/gosc/coverage.html
#scp  -P 21 /tmp/coverage.html  $USER:$PASS@$HOST:/gosc/
scp  -P 11 /tmp/coverage.html  root@192.168.1.1:/coverage.html
echo "koniec"
llvm-cov-3.9  report ../build-iDom_TESTs-CMAKE-clang-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-clang-Debug/test.profdata -use-color 
