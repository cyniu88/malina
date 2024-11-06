# #!/bin/sh
# echo "start"
# //../iDom_TESTs-CMAKE/iDom_TESTs-CMAKE
# echo "generuje raport"

# llvm-profdata-14  merge -o ../iDom_TESTs-CMAKE/test.profdata ../iDom_TESTs-CMAKE/default.profraw
# echo "tworze html"
# llvm-cov-14 show ../iDom_TESTs-CMAKE/iDom_TESTs-CMAKE -instr-profile=../iDom_TESTs-CMAKE/test.profdata  -path-equivalence -use-color --format html > /tmp/coverage.html
# echo "wysylam html na server"
# scp -P 11 /tmp/coverage.html  root@cyniu88.no-ip.pl:/mnt/usb/www_php/coverage.html
# echo "koniec"
# llvm-cov-14  report ../iDom_TESTs-CMAKE/iDom_TESTs-CMAKE -instr-profile=../iDom_TESTs-CMAKE/test.profdata -use-color | sed -e 's/\/home\/cyniu\/GIT\/malina\/iDom_server_OOP\///g' 
# llvm-cov-14 show ../iDom_TESTs-CMAKE/iDom_TESTs-CMAKE -instr-profile=../iDom_TESTs-CMAKE/test.profdata  -path-equivalence  --format text > coverage.txt
# llvm-cov-14 report ../iDom_TESTs-CMAKE/iDom_TESTs-CMAKE -instr-profile=../iDom_TESTs-CMAKE/test.profdata  > report.txt
# sed -i 's/\/home\/cyniu\/GIT\/malina\/iDom_server_OOP\///g' report.txt
# cp /tmp/coverage.html  ./coverage.html

#!/bin/sh
echo "start"

//../iDom_TESTs-CMAKE/iDom_TESTs-CMAKE

echo "Generuję raport"
llvm-profdata-14 merge -o ../iDom_TESTs-CMAKE/test.profdata ../iDom_TESTs-CMAKE/default.profraw

echo "Tworzę HTML"
llvm-cov-14 show ../iDom_TESTs-CMAKE/iDom_TESTs-CMAKE -instr-profile=../iDom_TESTs-CMAKE/test.profdata \
  --use-color --format=html > /tmp/coverage.html

echo "Wysyłam HTML na serwer"
scp -P 11 /tmp/coverage.html root@cyniu88.no-ip.pl:/mnt/usb/www_php/coverage.html

echo "Koniec"
llvm-cov-14 report ../iDom_TESTs-CMAKE/iDom_TESTs-CMAKE -instr-profile=../iDom_TESTs-CMAKE/test.profdata \
  --use-color | sed -e 's/\/home\/cyniu\/GIT\/malina\/iDom_server_OOP\///g'

llvm-cov-14 show ../iDom_TESTs-CMAKE/iDom_TESTs-CMAKE -instr-profile=../iDom_TESTs-CMAKE/test.profdata \
  --format=text > coverage.txt

llvm-cov-14 report ../iDom_TESTs-CMAKE/iDom_TESTs-CMAKE -instr-profile=../iDom_TESTs-CMAKE/test.profdata > report.txt
sed -i 's/\/home\/cyniu\/GIT\/malina\/iDom_server_OOP\///g' report.txt

cp /tmp/coverage.html ./coverage.html
