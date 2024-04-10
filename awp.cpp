//***********************************************|
// Auto Watering System                          |
//      DaAnHort JKI                             |
//          Yujie Zhang, anderskolby@hotmail.com |
//              2023-11-20                       |
//***********************************************|

// libs
#include <gpiod.h>
#include <unistd.h>
#include <iostream>
#include <csignal>
#include <fstream>
#include <chrono>
#include <ctime>

// std namespace save std::
using namespace std;

#define SENSOR_SWORD 17 // # for preprocess, no parameter define
#define RELAY_SWORD 18 // global

volatile sig_atomic_t flag = 0; // ez lose type and atomic for 32 and 64 bit flag
ofstream logFile; // write log

string getTimestamp() {
    auto now = chrono::system_clock::now(); // init auto
    time_t now_c = chrono::system_clock::to_time_t(now); //get time
    return string(ctime(&now_c)); // local time string
}

void output(const string &text) {
    string logText = getTimestamp() + ": " + text; // add timestamp to log
    cout << logText << endl; // terminal out
    logFile << logText << endl; // log out
}

void signal_handler(int sig) { // flag for signal control
    flag = 1;
}

int main() {
    int previous_water_level = -1; // initialize invalid value for water level change detection

    logFile.open("/home/yj/auto_watering/build/log/log.txt"); // save to log.txt

    if (!logFile.is_open()) {
        cout << "cannot open log file! check your right maybe chmod +x!" << endl;
        return 1; // error quit, can also be -1 but should pair with 0 in whole function
    }

    // offical Doc: struct gpiod_chip * gpiod_chip_open (const char *path)
    struct gpiod_chip *chip = nullptr;
    // offical Doc: struct gpiod_line * gpiod_chip_get_line (struct gpiod_chip *chip, unsigned int SWORD)
    struct gpiod_line *sensor_line = nullptr, *relay_line = nullptr;

    int ret = 0; // init ret out of loop

    // get signal type sigint for flag1
    signal(SIGINT, signal_handler); // csignal or signal.h, two function, signal to catch singla, raise to offer signal, 1st para. is singla type

    // load chip from pi
    chip = gpiod_chip_open("/dev/gpiochip0");

    //error check
    if (!chip) {
        output("cannot find gpio chip! check your pi at dev!");
        return 1;
    }

    //load sensor input
    sensor_line = gpiod_chip_get_line(chip, SENSOR_SWORD);

    if (!sensor_line) {
        output("cannot find sensor gpio line! check pin config!");
        goto cleanup;
    }

    // set initial pin17 value to high (1)
    //ret = gpiod_line_set_value(sensor_line, 1);

    //cout << ret << endl;
    //if (ret < 0) {
    //    output("cannot set initial value for the sensor line! is sensor connected?");
    //    goto cleanup;
    //}

    ret = gpiod_line_request_input(sensor_line, "sensor");

    if (ret < 0) {
        output("cannot request sensor line as input! check system!");
        goto cleanup;
    }

    relay_line = gpiod_chip_get_line(chip, RELAY_SWORD);

    if (!relay_line) {
        output("cannot find relay gpio line! check pin config!");
        goto cleanup;
    }

    ret = gpiod_line_request_output(relay_line, "relay", 0);

    if (ret < 0) {
        output("cannot request relay line as output! check system!");
        goto cleanup;
    }

    // while unlimited, for limited
    while (!flag) {
        int water_level = gpiod_line_get_value(sensor_line);
	//cout << water_level << endl;
        if (water_level == 1) {
            if (previous_water_level != 0) {
                gpiod_line_set_value(relay_line, 0);
                output("water level is low, pump running!");
                previous_water_level = 0;
            }
        }
        else {
            if (previous_water_level != 1) {
		output("signal received! run another 60s!");
		sleep(60); // adjust with sensor senstivity to get extra water level 
                gpiod_line_set_value(relay_line, 1);
                output("water level is normal, pump closed! sleep for 1h!");
                previous_water_level = 1; // when previous water level 0, quit this loop and search for 1 and -1

                sleep(1 * 60 * 60); // how long after flooding, default 6h
		//sleep(30);
            }
        }

        sleep(1);
    }

cleanup:

    if (sensor_line) gpiod_line_release(sensor_line);
    if (relay_line) gpiod_line_release(relay_line);
    if (chip) gpiod_chip_close(chip);

    logFile.close(); // close log file

    return 0;
}

