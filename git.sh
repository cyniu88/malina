#!/bin/sh
if [ $# == 0 ] ; then
echo "podaj nazwe commita!"
exit
fi
echo podano nazwe: $@
git add -A
sleep 1
git commit -a -m "$@"
sleep 1
git push
