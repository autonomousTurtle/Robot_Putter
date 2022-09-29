#!/usr/bin/env python

import time
import serial
import tty, sys, termios, select
import Gamepad
#sys.path.insert(0, "/home/pi/")

# GLOBAL VARIABLES
# set frame rate - how many cycles/second to run the loop
FRAMERATE = 15
# how long does each frame take in seconds?
FRAME = 1.0/FRAMERATE
# initialize myTimer
topOfFrame = 0.0
endOfWork = 0.0
endOfFrame = 0.0
# how many cycles to test? counter*FRAME = runtime in seconds
counter = 2000
# fudge factor for our timing loop computations
TIME_CORRECTION = 0.0

timeError = 0.0
startup = True
CANCEL_MOVE_COUNT = 4 #number of loops unitl we send a 0 drive command
cancl_move_counter = 0

full_speed = 255 # what is the full speed of the robot, max is 255
current_speed_L = 0 # keep track of how fast the left motor is moving
current_speed_R = 0 # keep track of how fast the right motors is moving 
desired_speed = 0 # keep track of how fast the right motors is moving
current_speed = 0 # keep track of how fast the right motors is moving

prev_drive_messge_sent = False # track if we previously sent a drive message so we can send 0's if we want to stop

# controller setup
buttonHappy = 'CROSS'
buttonBeep = 'CIRCLE'
buttonExit = 'PS'
joystickLeftRight = 0 # right and left on D pad
joystickUpDown = 1 # up and down on D pad

dPadLeftRight = 6 # right and left on D pad
dPadUpDown = 7 # up and down on D pad

pollInterval = 0.2

# this class encapsulates our robot
class Robot():
    def __init__(self):
        # used to send commands to arduino and motors
        self.robotCommand = False  # is there a new command to send to robot?
        self.lastHeartBeat = 0.0  # time of the last heartbeat from Arduino

    def recHearBeat(self, hbTime):
        hbRecieved = time.time()
        self.latency = hbRecieved - hbTime
        self.lastHeartBeat = hbRecieved

    def vectorize(self):
        # this turns position, velocity, and accel into a vector
        self.vecX = [self.x, self.vx, self.ax]
        self.vecY = [self.y, self.vy, self.ay]
        self.ang = [self.yaw, self.vYaw, self.aYaw]


def readBuffer(buff):
    # the data we get is a series of lines separated by EOL symbols
    # we read to a EOL character (0x10) that is a line
    # process complete lines and save partial lines for later

    EOL = '>'
    if len(buff) == 0:
        return
    dataLine = ""
    lines = []
    for inChar in buff:
        if inChar != EOL:
            dataLine += inChar
        else:
            lines.append(dataLine)
            dataLine = ""
    for telemetryData in lines:
        processData(telemetryData)
    return dataLine


def processData(dataLine):
    # take information from the arduino and process telemetry into
    # status information
    # we recieve either heartbeat (HBB), telemetry list 1 (TL1), or error messages (ERR)

    dataType = dataLine[:3]
    payload = dataLine[3:]  # rest of the line is data
    if dataType == 'HBB':
        print(f"Heartbeat received: {payload}")
        
    if dataType == 'TL1':
        # process telemetry data
        pass
    if dataType == 'ERR':
        # process error data
        print("ARDUINO ERROR MESSSAGE ", payload)
        # log the error
        #rospy.loginfo(payload)
    return

# grab strokes from the keyboard and send them over
def isData():
    return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

def leftRightAxisMoved(position):
    global leftRight
    leftRight = position   # Inverted
    print(leftRight)

def upDownAxisMoved(position):
    global upDown
    upDown = position # Non-inverted
    print(upDown)

def getSpeed(postion):
    global current_speed_R
    global current_speed_L

# initialize the joysticks
if not Gamepad.available():
    print('Please connect your gamepad...')
    while not Gamepad.available():
        time.sleep(1.0)
gamepad = Gamepad.Gamepad()
print('Gamepad connected')
gamepad.startBackgroundUpdates()


# main program start here
# *********************************************************************
robot = Robot()  # define our robot instance
serialPort = "/dev/ttyACM0"
# open the serial port to arduino
ser = serial.Serial(serialPort,115200,timeout=0)
# serial port with setting 38,400 baud, 8 bits, No parity, 1 stop bit=
ser.reset_input_buffer()

# Set some initial state
leftRight = 0.0
upDown = 0.0
gamepad.addAxisMovedHandler(joystickLeftRight, leftRightAxisMoved)
gamepad.addAxisMovedHandler(joystickUpDown, upDownAxisMoved)

frameKnt = 0  # counter used to time processes
newDriveMessage = False
newPuttMessage = False

try:
    while True:

        if not Gamepad.available():
            print('Please connect your gamepad...')
            while not Gamepad.available():
                time.sleep(1.0)
            gamepad = Gamepad.Gamepad()
            print('Gamepad connected')
            gamepad.startBackgroundUpdates()

        # main loop
        topOfFrame = time.time()

        # read data from serial port
        if ser.in_waiting > 0:
            serData = ser.readline().decode('utf-8').rstrip()
            if len(serData) > 0:
                print(f"serial data: {serData}")
        # process the data from the arduino
        # read and parse the lines to avoid any blocking
            holdBuffer = readBuffer(serData)

        com = ','
        EOL = '>'

        # check if the controller has been used
        if gamepad.beenPressed(0): # A
            print("A Pressed")
            robMessage = "DRV"+com+str(0)+com+str(0)+EOL
            newDriveMessage = True
        if gamepad.beenPressed(1): # B
            print("B Pressed")
            newPuttMessage = True
        if gamepad.beenReleased(0): # A
            print("A Released")
        if gamepad.beenReleased(1): # B
            print("B Released")

        # set speeds, range (0-255)
        if upDown > .25 or upDown < -.25:
            desired_speed = int(full_speed * abs(upDown))
        elif leftRight < -.25 or leftRight > .25:
            desired_speed = int(full_speed * abs(leftRight))
        else:
            desired_speed = 0

        #print("Desired speed: ", desired_speed)

        if desired_speed > full_speed:
            desired_speed = full_speed
        if desired_speed > 0 and desired_speed < 60:
            desired_speed = 60

        current_speed = desired_speed

        if upDown > .3:
            if leftRight > .3:
                print("right and down")
                robMessage = "DRV"+com+str(desired_speed)+com+str(-int(desired_speed/2))+EOL
            elif leftRight < -.3:
                print("left and down")
                robMessage = "DRV"+com+str(int(desired_speed/2))+com+str(-desired_speed)+EOL
            else:
                print("down")
                robMessage = "DRV"+com+str(desired_speed)+com+str(-desired_speed)+EOL
            newDriveMessage = True
        elif upDown < -.3:
            if leftRight > .3:
                print("right and up")
                robMessage = "DRV"+com+str(-desired_speed)+com+str(int(desired_speed/2))+EOL
            elif leftRight < -.3:
                print("left and up")
                robMessage = "DRV"+com+str(-int(desired_speed/2))+com+str(desired_speed)+EOL
            else:
                print("up")
                robMessage = "DRV"+com+str(-desired_speed)+com+str(desired_speed)+EOL
            newDriveMessage = True
        else:
            if leftRight > .3:
                print("spin right")
                robMessage = "DRV"+com+str(-desired_speed)+com+str(-desired_speed)+EOL
                newDriveMessage = True
            elif leftRight < -.3:
                print("spin left")
                robMessage = "DRV"+com+str(desired_speed)+com+str(desired_speed)+EOL
                newDriveMessage = True
            else:
                current_speed = 0


        if newDriveMessage:
            print(f"Wrote to Arduino:{robMessage}") # for debug purposes
            ser.write(str.encode(robMessage))
            ser.flush() # output Now
            newDriveMessage = False
            cancl_move_counter= 0
            prev_drive_messge_sent = True # we just sent a drive message

        elif prev_drive_messge_sent == True:
            robMessage = "DRV"+com+str(0)+com+str(0)+EOL
            current_speed = 0
            ser.write(str.encode(robMessage))
            ser.flush() # output Now
            prev_drive_messge_sent = False
            newDriveMessage = False
        
        elif newPuttMessage == True:
            robMessage = "P"+com+str(0)+com+str(0)+EOL
            ser.write(str.encode(robMessage))
            ser.flush() # output Now
            newPuttMessage = False 

        elif frameKnt % (FRAMERATE) == 0:  # once per second
            hbMessage = "HBB"+com+str(time.time())[-7:]+EOL
            print(hbMessage)
            ser.write(str.encode(hbMessage))
            ser.flush()

        frameKnt += 1
        # reset the frame count if it is the same as the frameRate
        if frameKnt == FRAMERATE:
            frameKnt = 0

        # just count the number of frames in a second
        frameKnt = min(FRAMERATE, frameKnt)

        # done with work, now make timing to get our frame rate to come out right

        # may have to work on PID of time Error - currently it is unstable process, will eventually result in a sleep error
        endOfWork = time.time()
        workTime = endOfWork - topOfFrame
        sleepTime = (FRAME-workTime)
        if sleepTime> 0:
            time.sleep(sleepTime)
        endOfFrame = time.time()
        startup = False
        # end of the main loop
        

finally:
    gamepad.disconnect()
    ser.close()