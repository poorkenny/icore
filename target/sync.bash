#!/bin/bash
# 
# File:   freeze.bash
# Author: stan
#
# Created on Jun 12, 2013, 4:04:55 PM
#

#use bash to execute this script
wget "http://localhost:20000/control/pauseat=$clock"
rm pauseat*
var="synctime:"
var1="$var$clock"
echo $var1
exec 3<> /dev/tcp/127.0.0.1/9019
echo -e $var1 >&3
# echo "set new value:"
# wget http://localhost:10000/my_house/cooling_setpoint=91 -q
# echo "get new value:"
# wget http://localhost:10000/my_house/cooling_setpoint -q -O -
# wget "http://localhost:10000/control/resume"