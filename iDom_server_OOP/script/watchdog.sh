#!/bin/bash
var=`ps ax| grep iDom_server | wc | awk '{print$1}'`
echo  var : $var

if  [ $var != 2 ]
then
sudo  chmod 777 /tmp/iDom_log.log
sudo echo -e "\nwatchdog zadzialal\n" 

screen -S dom  -d -m sudo /home/pi/programowanie/iDom_server_OOP-build-clang-Release/iDom_server_OOP

fi
