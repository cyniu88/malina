#!/bin/bash
var=`ps ax| grep iDom_server | wc | awk '{print$1}'`
if  [ $var != 2 ]
then
sudo  chmod 777 /tmp/iDom_log.log
sudo echo -e "\nwatchdog zadzialal\n" >> /tmp/iDom_log.log

screen -S dom  -d -m sudo /home/pi/programowanie/iDom_server-build-desktop-Qt_4_8_2__System__Release/iDom_server

fi