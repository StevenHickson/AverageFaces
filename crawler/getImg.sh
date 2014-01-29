#!/bin/bash

while read line; do
  link=`echo $line | grep fullImageLink`
  if [[ -n $link ]]; then
    var=${link#*upload*}
    var=${var%*\"><*}
    if [[ -n $var ]]; then
      cd images
      wget "https://upload$var"
      cd ../
    fi
  fi
done < tmp
