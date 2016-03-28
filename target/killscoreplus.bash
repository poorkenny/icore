#!/bin/bash
# 
# File:   
# Author: stan
#
# Created on Jun 12, 2013, 4:04:55 PM
#

#use bash to execute this script

# var="session completed!"
# var1="$var"
# exec 3<> /dev/tcp/127.0.0.1/9019
# echo -e $var1 >&3

wget "http://localhost:20000/control/shutdown"

var2=$(ps aux | awk '/\/tmp\/pycore/{print $(NF)}' | head -1)
coreSessionId=${var2:12:5}
core --closebatch $coreSessionId
# 
# var2=$(ps aux | awk '/\/tmp\/pycore/{print $(NF)}' | head -1)
# coreSessionId=${var2:12:5}
# core --closebatch $coreSessionId
kill `ps h -o pid -C syncontroller`
kill `ps h -o pid -C vtcontroller`

echo "session has been killed successfully!"


