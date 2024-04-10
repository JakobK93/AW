#!/bin/bash

photo_dir="/home/yj/auto_watering/build/photo"
photo_name=$(date +"%Y%m%d%H%M%S").jpg
raspistill -w 1440 -h 1080 -o "$photo_dir/$photo_name"
