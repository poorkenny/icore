#!/bin/bash
# 
# File:   scoreplusclean.bash
# Author: stan
#
# Created on Jul 19, 2013, 2:34:00 PM
#
sudo core-cleanup.sh

sudo rm /tmp/vt*

wget http://localhost:20000/control/shutdown

cd /home/stan/code/scoreplus/trunk/nodeexec

make

ps aux | awk '/\/home\/stan\/code\/scoreplus\/trunk\/syncontroller\/dist/{  print $2 }' | xargs kill -9
ps aux | awk '/\/home\/stan\/code\/scoreplus\/trunk\/vtcontroller\/dist/{  print $2 }' | xargs kill -9

ps aux | awk '/\/usr\/local\/sbin\/cored/{  print $2 }' | sudo xargs kill -9

sudo /etc/init.d/core start

sudo rm /tmp/ss*