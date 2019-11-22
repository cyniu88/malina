#!/bin/bash

git pull

if [ $? = 0 ] ; then
echo  "git pull : $?"
else
echo  "błąd: $?  git pull "
exit 1

fi

git submodule update

if [ $? = 0 ] ; then
echo  "git submodule update : $?"
else
echo  "błąd: $?  git submodule update "
exit 1

fi

cd iDom_TESTs-CMAKE/ 

if [ $? = 0 ] ; then
echo  "cd iDom_TESTs-CMAKE/  : $?"
else
echo  "błąd: $?  cd iDom_TESTs-CMAKE/"
exit 1

fi


cmake .


if [ $? = 0 ] ; then
echo  "cmake .  : $?"
else
echo  "błąd: $?  cmake ."
exit 1

fi


make -j2


if [ $? = 0 ] ; then
echo  "make -j2  : $?"
else
echo  "błąd: $?  make -j2"
exit 1
fi


./iDom_TESTs-CMAKE


if [ $? = 0 ] ; then
echo  "iDom_TESTs-CMAKE  : $?"
else
echo  "błąd: $?  iDom_TESTs-CMAKE"
exit 1
fi


pwd
cd ../../cmake/iDom_server-CMAKE/


if [ $? = 0 ] ; then
echo  "cd ../../cmake/iDom_server-CMAKE/  : $?"
else
echo  "błąd: $?  cd ../../cmake/iDom_server-CMAKE/"
exit 1
fi


make -j2


if [ $? = 0 ] ; then
echo  "make -j2  : $?"
else
echo  "błąd: $?  make -j2"
exit 1
fi

