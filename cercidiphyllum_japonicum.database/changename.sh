#!/bin/bash

#acer_ginnala

var=0

for file in *.jpg
do
  var=$((var+1))
  str="cer_jap${var}.jpg"
  mv $file $str
done