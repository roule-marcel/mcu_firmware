#!/bin/bash

DEV=/dev/ttyAMA0
BIN=$1

echo -ne "b\r" > $DEV
sleep 1
cat $1 > $DEV
