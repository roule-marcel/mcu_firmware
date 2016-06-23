#!/bin/bash

BIN=$1

echo -ne "b\r" > /dev/rfcomm0
sleep 1
cat $1 > /dev/rfcomm0
