#!/usr/bin/env python

import time
import serial
import tty, sys, termios, select
import pygame

# GLOBAL VARIABLES
# set frame rate - how many cycles/second to run the loop
FRAMERATE = 30
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
        rospy.loginfo(payload)
    return


# grab strokes from the keyboard and send them over
def isData():
    return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

filedescriptors = termios.tcgetattr(sys.stdin)
tty.setcbreak(sys.stdin)
old_settings = termios.tcgetattr(sys.stdin)


# main program start here
# *********************************************************************
robot = Robot()  # define our robot instance
serialPort = "/dev/ttyACM0"
# open the serial port to arduino
ser = serial.Serial(serialPort,115200,timeout=0)
# serial port with setting 38,400 baud, 8 bits, No parity, 1 stop bit=
'''
try:
    ser.open()
except:
    print("SERIAL PORT FOR ARDUINO DID NOT OPTN ", serialPort)
    raise
'''

frameKnt = 0  # counter used to time processes
while True:

    # main loop
    topOfFrame = time.time()
    #print (time)

    # read data from serial port
    serData = ser.readline().decode('utf-8').rstrip()
    if len(serData) > 0:
        print(f"serial data: {serData}")
    # process the data from the arduino
    # read and parse the lines to avoid any blocking
    holdBuffer = readBuffer(serData)

    com = ','
    EOL = '>'

    # grab data from the keyboard and send to the motors
    if isData():
        c = sys.stdin.read(1)
        if c == '\x1b':         # x1b is ESC
            robMessage = "DRV"+com+str(0)+com+str(0)+EOL
            ser.write(str.encode(robMessage))
            ser.flush() # output Now
            break
        newDriveMessage = False
        if c == 'w':
            robMessage = "DRV"+com+str(255)+com+str(255)+EOL
            newDriveMessage = True
        elif c == 'a':
            robMessage = "DRV"+com+str(-150)+com+str(150)+EOL
            newDriveMessage = True
        elif c == 's':
            robMessage = "DRV"+com+str(-200)+com+str(-200)+EOL
            newDriveMessage = True
        elif c == 'd':
            robMessage = "DRV"+com+str(150)+com+str(-150)+EOL
            newDriveMessage = True
        elif c == 'z':
            robMessage = "DRV"+com+str(0)+com+str(0)+EOL
            newDriveMessage = True
        if newDriveMessage:
            print(f"Wrote to Arduino:{robMessage}") # for debug purposes
            ser.write(str.encode(robMessage))
            ser.flush() # output Now
            newDriveMessage = False
        

    if frameKnt % (FRAMERATE/1) == 0:  # twice per second
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

    #print (timeError)

    # may have to work on PID of time Error - currently it is unstable process, will eventually result in a sleep error
    endOfWork = time.time()
    workTime = endOfWork - topOfFrame
    sleepTime = (FRAME-workTime)
    if sleepTime> 0:
        time.sleep(sleepTime)
    endOfFrame = time.time()
    #actualFrameTime = endOfFrame-topOfFrame
    #timeError = FRAME-actualFrameTime
    # clamp the tim error to the size of the frame
    #timeError = min(timeError, FRAME)
    #timeError = max(timeError, -FRAME)
    startup = False
    # end of the main loop
    #
ser.close()