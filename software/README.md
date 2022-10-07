# SOFTWARE 

## Overview: 

This robot uses [subsumption control architecure](https://en.wikipedia.org/wiki/Subsumption_architecture) to manage both a Raspberry Pi and an Arduino microcontroller. The Arduino is responsible for the low level management: motor control, servo control, LED control. The Raspberry Pi is responsible for the higer level control: talking to the XBox controller, ensure the robot si driving straight, when to move the putter vs when to move the motors, speed ramping and more. 

In this architecture, the raspberry pi is sending strings of commands to the Arduino. The Arduino looks at the commands and executes the necessary code to make the desired actions happen. When there are no actions called for from the Pi, they communicate back and forth at 2hz with a heartbeat. If the Pi or Arduino fail to send or recieve a heartbeat signal, they will stop all motion and shut down their processes. This is an important safety mechanism to prevent run-away robots! 

## Code: 
**Python Script run on Raspberry Pi:** [drive_robot.py](https://github.com/autonomousTurtle/Robot_Putter/blob/main/software/deployed/drive_robot.py)


_this code will start automatically with an auto-run script, but to run manually through ssh, navigate to the root folder and type:_

`$ python3 drive_robot.py`

**Arduino Script:** [drive_and_putt.ino](https://github.com/autonomousTurtle/Robot_Putter/blob/main/software/drive_and_putt/drive_and_putt.ino)

## Rapsberry Pi Connections:

### LAN
Raspberry Pi LAN IP: 192.168.7.21

### WAN and SSH
username: pi@192.168.7.129
password: raspberry


## Configuring the Raspberry Pi:

### Serial
You need to configure serial on the Pi to be able to talk to Aruduino board. You do this by giving the user dialout permissions:

`usermod -a -G dialout $USER`


### Auto Run Python Script 
We need to automatically run the drive_robot.py script on boot of the rpi. [Link here is hlepful to get it set up](https://www.codementor.io/@ufukafak/how-to-run-a-python-script-in-linux-with-systemd-1nh2x3hi0e_)


## Pi Libraries:

### Read Controller Inputs
[PiBorg Gamepad Library](https://www.piborg.org/blog/gamepad-library): [Github Link](https://github.com/piborg/Gamepad)

This library is great becuases it has built in asynchronous updates in a background thread. 

```
import Gamepad
gamepad = Gamepad.PS4()

# Start the background updating
gamepad.startBackgroundUpdates()

# Your code here :)

# Stop the background thread
gamepad.disconnect()
```


