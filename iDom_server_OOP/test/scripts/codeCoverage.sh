#!/bin/bash
../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/iDom_TESTs-CMAKE
echo "generuje raport"
llvm-profdata-3.9  merge -o ../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/test.profdata ../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/default.profraw
echo "tworze html"

llvm-cov-3.9 show ../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/test.profdata  -filename-equivalence -use-color --format html > /tmp/coverage.html
echo "wysylam html na server"
scp -P 11 /tmp/coverage.html  root@cyniu88.no-ip.pl:/www_php/coverage.html
echo "koniec"
llvm-cov-3.9  report ../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/test.profdata -use-color | sed -e 's/\/home\/cyniu\/GIT\/malina\/iDom_server_OOP\///g' 

llvm-cov-3.9 show ../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/test.profdata  -filename-equivalence  --format text > coverage.txt
llvm-cov-3.9 report ../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Zaimportowany_zestaw_narz_dzi_tymczasowy-Debug/test.profdata  > report.txt
sed -i 's/\/home\/cyniu\/GIT\/malina\/iDom_server_OOP\///g' report.txt
cp /tmp/coverage.html  ./coverage.html
