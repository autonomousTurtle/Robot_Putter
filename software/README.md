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


### Motor Driver


### 

