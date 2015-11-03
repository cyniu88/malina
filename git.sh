#!/bin/sh
if [ $# == 0 ] ; then
echo "podaj nazwe commita!"
exit
fi
echo podano nazwe: $@
git add .
git commit -a -m $@
git push
