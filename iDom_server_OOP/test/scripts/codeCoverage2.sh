#!/bin/sh
echo "start"
../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/iDom_TESTs-CMAKE
echo "generuje raport"

llvm-profdata-6.0  merge -o ../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/test.profdata ../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/default.profraw
echo "tworze html"
llvm-cov-6.0 show ../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/test.profdata  -path-equivalence -use-color --format html > /tmp/coverage.html
echo "wysylam html na server"
scp -P 11 /tmp/coverage.html  root@cyniu88.no-ip.pl:/www_php/coverage.html
echo "koniec"
llvm-cov-6.0  report ../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/test.profdata -use-color | sed -e 's/\/home\/cyniu\/GIT\/malina\/iDom_server_OOP\///g' 
llvm-cov-6.0 show ../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/test.profdata  -path-equivalence  --format text > coverage.txt
llvm-cov-6.0 report ../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/iDom_TESTs-CMAKE -instr-profile=../build-iDom_TESTs-CMAKE-Imported_Kit-debuge/test.profdata  > report.txt
sed -i 's/\/home\/cyniu\/GIT\/malina\/iDom_server_OOP\///g' report.txt
cp /tmp/coverage.html  ./coverage.html
