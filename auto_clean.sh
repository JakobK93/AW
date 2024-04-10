#!/bin/bash

dir="./build/log"
files=`ls ${dir}`
for file in $files
do
if [ -e ${dir}/${file} ];then
rm -f ${dir}/${file}
fi
done
