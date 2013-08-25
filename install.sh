#!/bin/bash

RULES_DIR=/etc/udev/rules.d/
LIB_DIR=/usr/lib/
INC_DIR=/usr/include/

cp other/99-RGBLEDController.rules $RULES_DIR
cp lib/unix/librgbledctrl.so $LIB_DIR
cp lib/unix/librgbledctrl.a $LIB_DIR
cp include/rgbledctrl.h $INC_DIR