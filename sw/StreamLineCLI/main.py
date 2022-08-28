import serial.tools.list_ports
from datetime import datetime
import curses
from curses import wrapper
import time
import re

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

portList = []

def main(stdscr):
    stdscr.clear()
    stdscr.addstr("Press any key to continue...")
    stdscr.getch()
    stdscr.clear()
    stdscr.addstr("Select [Number] of COM port to connect:\n\r")

    for onePort in ports:
        portList.append(str(onePort))
        stdscr.addstr(str(onePort))

    val = stdscr.getkey()

    for x in range(0, len(portList)):
        if portList[x].startswith("COM" + str(val)):
            portVar = "COM" + val
            stdscr.clear()
            stdscr.addstr(str(portList[x]))
            stdscr.refresh()

    # Create list
    dataDict = {}
    tempList = []

    # Serial Configuration
    serialInst.baudrate = 115200
    serialInst.port = portVar
    serialInst.open()

    stdscr.clear()
    stdscr.refresh()
    while True:
        if serialInst.in_waiting:
            packet = serialInst.readline()
            if packet[0] == 0:
                now = datetime.now()
                current_time = now.strftime("%H:%M:%S")
                tempDict = {current_time: tempList.copy()}
                dataDict.update(tempDict)
                stdscr.clear()
                for x in range(len(tempList)):
                    stdscr.addstr(x, 0, str(tempList[x]))
                    stdscr.refresh()
                tempList.clear()
                continue
            my_string = str(packet.decode('utf').rstrip('\n'))
            my_string = my_string.replace('\0','')
            tempList.append(my_string)

wrapper(main)
