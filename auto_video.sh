#!/bin/bash

video_dir="./build/video"
video_name=$(date +"%Y%m%d%H%M%S").h264
raspivid -w 1440 -h 1080 -o "$video_dir/$video_name" -t 5000  # 5s
