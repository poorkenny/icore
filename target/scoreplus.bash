#!/bin/bash
# 
# File:   score.bash
# Author: stan
#
# Created on Jul 4, 2013, 3:13:13 PM
#
#vnodeNumber=10
#virtualStartTime="2013-05-01 00:00:00"
#virtualStopTime="2013-05-01 00:10:00"
#var1="create new session|$vnodeNumber|$virtualStartTime|$virtualStopTime|"
CURRENT_DIRECTORY=${PWD}
#if [[ -f "$1" && -f "$2" && -f "$3" ]]
#then
if [[ -f "$1" && -f "$2" ]]
then
#start core, .imn file
#echo "Set up core...."
#core --batch "$CURRENT_DIRECTORY/$2"

#set up synccontroller and vtcontroller
echo "Set up syncontroller and vtcontroller..."
parentdir="$(dirname "$CURRENT_DIRECTORY")"
$parentdir/syncontroller/dist/Debug/GNU-Linux-x86/syncontroller
$parentdir/vtcontroller/dist/Debug/GNU-Linux-x86/vtcontroller
sleep 2
var1="$CURRENT_DIRECTORY/$1"
#var1="$1"
cp $var1 /tmp/scoreplus.session
exec 3<> /dev/tcp/127.0.0.1/9019
echo -e $var1 >&3
#close read/write file descriptor 3 -- shane
exec 3<&-
exec 3>&-
#The sequence can not be changed.
sleep 5
echo "Starting gridlabd..."
#start gridlabd, .glm file
echo 
gridlabd "$CURRENT_DIRECTORY/$2" --server 
#gridlabd "$2" --server

else
  echo "some input files not found!"
  echo "there should be .sync .glm 2 input files in order."
fi
