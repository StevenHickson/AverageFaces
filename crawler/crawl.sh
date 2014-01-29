#!/bin/bash

mkdir -p images
wget -O congress https://en.wikipedia.org/wiki/Current_members_of_the_United_States_House_of_Representatives

while read line; do
  var=${line#*href=\"*}
  var=${var%*\" class*}
  link=`echo "$var" | grep jpg`
  if [[ -n $link ]]; then
    wget -O tmp "https://en.wikipedia.org$link"
    ./getImg.sh
  fi
done < congress
