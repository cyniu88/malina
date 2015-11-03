#!/bin/sh
if [ $# != 1 ] ; then
echo "podaj nazwe commita!"
exit
fi
git add .
git commit -a -m $1
git push
