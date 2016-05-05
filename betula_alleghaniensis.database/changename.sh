#!/bin/bash

#acer_ginnala

var=0

for file in *.jpg
do
  var=$((var+1))
  str="betula_all${var}.jpg"
  mv $file $str
done