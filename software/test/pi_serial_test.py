#!/usr/bin/env python3

# This scripts tests the communication with the ARDUINO BOARD
# Upload Serial_comtest.ino to the arduino board for bi-directional comms

import serial
import tty, sys, termios, select
import time
import pygame

filedescriptors = termios.tcgetattr(sys.stdin)
tty.setcbreak(sys.stdin)
x = 0


def isData():
    return select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], [])

old_settings = termios.tcgetattr(sys.stdin)

if __name__ == '__main__':
    tty.setcbreak(sys.stdin.fileno())
    # set the serail port and baudrate to match the arduino
    ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
    ser.reset_input_buffer()
    i = 0
    while True:

        # read inbound messages from the arduino
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line) # print messages from arduino to termianl

        # read the keyboard and send any characters through serial
        if isData():
            c = sys.stdin.read(1)
            if c == '\x1b':         # x1b is ESC
                break
            # send the data to the arduino 
            print(f"Wrote to Arduino:{c}")
            ser.write(str.encode(c))
            

    termios.tcsetattr(sys.stdin, termios.TCSADRAIN,filedescriptors)
    