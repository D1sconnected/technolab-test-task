import serial.tools.list_ports
from datetime import datetime
import curses
from curses import wrapper
import time
import re

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

portList = []

DEFAULT_BAUDRATE = 115200

HINTS_WINDOW_HEIGHT = 6
HINTS_WINDOW_WIDTH  = 80
HINTS_WINDOW_X_POS  = 0
HINTS_WINDOW_Y_POS  = 0

DATA_WINDOW_HEIGHT = 15
DATA_WINDOW_WIDTH  = 50
DATA_WINDOW_X_POS  = 0
DATA_WINDOW_Y_POS  = HINTS_WINDOW_HEIGHT

def main(stdscr):

    # Display Stream data content
    data_window = curses.newwin(DATA_WINDOW_HEIGHT, DATA_WINDOW_WIDTH, DATA_WINDOW_Y_POS, DATA_WINDOW_X_POS)

    # Display hints, shortkeys and other info
    hints_window = curses.newwin(HINTS_WINDOW_HEIGHT, HINTS_WINDOW_WIDTH, HINTS_WINDOW_Y_POS, HINTS_WINDOW_X_POS)

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
    # Create list to store current frame
    frame = []

    # Serial Configuration
    serialInst.baudrate = DEFAULT_BAUDRATE
    serialInst.port = portVar
    serialInst.open()

    stdscr.clear()
    stdscr.refresh()

    # Display hints
    hints_window.clear()
    hints_window.addstr('Connected to: {} '.format(portVar))
    hints_window.addstr('with {} baudrate\n'.format(DEFAULT_BAUDRATE))
    hints_window.addstr('-----------------------------------------------------------------------\n')
    hints_window.addstr('Press [NUMBER] to ENABLE/DISABLE output or [KEY] to toggle state\n')
    hints_window.addstr('[1]ADC0 [2]ADC1 [3]LED0 [4]LED1 [5]LED2 [6]LED3 [7]BTN0 [8]HLD0 [9]TMP0\n')
    hints_window.addstr('[B]LED0 [R]LED1 [O]LED2 [G]LED3\n')
    hints_window.addstr('-----------------------------------------------------------------------')
    hints_window.refresh()

    while True:
        if serialInst.in_waiting:
            packet = serialInst.readline()
            if packet[0] == 0:
                now = datetime.now()
                current_time = now.strftime("%H:%M:%S")
                data_window.clear()
                for x in range(len(frame)):
                    data_window.addstr(x, 0, str(frame[x]))
                    data_window.refresh()
                frame.clear()
                continue
            my_string = str(packet.decode('utf').rstrip('\n'))
            my_string = my_string.replace('\0','')
            frame.append(my_string)

wrapper(main)
