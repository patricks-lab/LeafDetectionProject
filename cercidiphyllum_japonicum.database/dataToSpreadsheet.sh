#!/bin/bash

for file in *.convex
do
  value=`cat $file`
  echo $value
done