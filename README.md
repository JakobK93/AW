# 💧Auto Watering System based on Machine Learning🧠

This project is for embedded system pump control.
You need float-kind water level sensor, micro-controller, water pump, relay, wires, and two water tanks.

Volt choice depends on which kind of pump, micro-controller, realy, pump, wires.
PS: DO NOT BURN YOUR HOUSE! 💔

If you use my project, please cite this github link to support open source power. Thanks a lot! 

## Main Tasks: 

- Camera Streaming 
- Sensor Detection and Signal Transfer to Micro-controller (MC) 
- MC Signal Reciver and MC Signal Sender to Pump through relay
- Crontab for period task 
- Auto detection with machine learning (developing)

## How to Start: 
```sh
sudo apt update && sudo apt -y upgrade
sudo apt-get autoremove -y
```

```sh
sudo apt install libgpiod-dev cron cmake
mkdir build && cd build
cmake ..
make
./awp
```

## For Period Task
You may need to adjust which shell you need for your purpose, and adjust the gpio pre-setting with auto gpio shell based on your electric connection. 
If you need more help, please contact me at: yujie.zhang@juliuse-kuehn.de

```sh
// check exist task
crontab -l
// setup your tasks 
crontab -e 

