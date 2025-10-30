#!/bin/bash
cd ../..
make lin
echo __DEBUG-------------------------------------------
#cd build/bin
gdb -q build/bin/wlh 
#xfce4-terminal -e 'gdb exe' -T "wlh" --maximize
