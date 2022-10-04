# SOFTWARE 


## Connections
Pi IP: 192.168.7.21
SSH Username: pi@192.168.7.129


## Libraries:

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

### Serial
You need to configure serial on the Pi to be able to talk to Aruduino board. You do this by giving the user dialout permissions:

`usermod -a -G dialout $USER`


### Auto Run Python Script 
We need to automatically run the drive_robot.py script on boot of the rpi. [Link here is hlepful to get it set up](https://www.codementor.io/@ufukafak/how-to-run-a-python-script-in-linux-with-systemd-1nh2x3hi0e_)


### Motor Driver


### 

