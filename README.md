# EE183DA Final Project - HAPI

**Installation**
1. Setup the RPI using this guide:
https://www.raspberrypi.org/documentation/installation/noobs.md

2. Wire the arduino according to the following wiring diagram:
![35301243_10213314346025806_4738161174523150336_n](https://user-images.githubusercontent.com/36172219/41448674-db14ad48-7010-11e8-9fd0-df1015b95ac9.png)

3. In the RPI, install Flask using the following tutorial:
https://projects.raspberrypi.org/en/projects/python-web-server-with-flask

4. Connect the Arduino to the RPI using one of its USB ports.

5. The webserver can then be run using the command
```
python app.py
```

**Requirements**

The requirements include a Raspberry Pi 3 and an Arduino. Various sensors are also necessary, and a list can be found in the report.

**Overall Implementation Summary**

The HAPI robot is an automated pill dispenser. It runs a web server to collect user input, uses a LCD screen to provide HRI feedback, and communicates with an Arduino over serial to dispense pills.

**Video Demo**

https://www.youtube.com/watch?v=G2eGzNecvgg
