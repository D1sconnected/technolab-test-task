import serial.tools.list_ports
from datetime import datetime
import curses
from curses import wrapper
import time
import re

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

portList = []

HINTS_ADC0_OUTPUT_KEY = ord('1')
HINTS_ADC1_OUTPUT_KEY = ord('2')
HINTS_LED0_OUTPUT_KEY = ord('3')
HINTS_LED1_OUTPUT_KEY = ord('4')
HINTS_LED2_OUTPUT_KEY = ord('5')
HINTS_LED3_OUTPUT_KEY = ord('6')
HINTS_BTN0_OUTPUT_KEY = ord('7')
HINTS_HLD0_OUTPUT_KEY = ord('8')
HINTS_TMP0_OUTPUT_KEY = ord('9')

HINTS_LED0_STATE_KEY  = ord('b')
HINTS_LED1_STATE_KEY  = ord('r')
HINTS_LED2_STATE_KEY  = ord('o')
HINTS_LED3_STATE_KEY  = ord('g')

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

    stdscr.nodelay(True)
    try:
        while True:
            # Capture if a key was pressed
            pressed_key = stdscr.getch()

            if pressed_key == HINTS_ADC0_OUTPUT_KEY:
                exit()
            if pressed_key == HINTS_ADC1_OUTPUT_KEY:
                exit()
            if pressed_key == HINTS_LED0_OUTPUT_KEY:
                exit()
            if pressed_key == HINTS_LED1_OUTPUT_KEY:
                exit()
            if pressed_key == HINTS_LED2_OUTPUT_KEY:
                exit()
            if pressed_key == HINTS_LED3_OUTPUT_KEY:
                exit()
            if pressed_key == HINTS_BTN0_OUTPUT_KEY:
                exit()
            if pressed_key == HINTS_HLD0_OUTPUT_KEY:
                exit()
            if pressed_key == HINTS_TMP0_OUTPUT_KEY:
                exit()
            if pressed_key == HINTS_LED0_STATE_KEY:
                exit()
            if pressed_key == HINTS_LED1_STATE_KEY:
                exit()
            if pressed_key == HINTS_LED2_STATE_KEY:
                exit()
            if pressed_key == HINTS_LED3_STATE_KEY:
                exit()

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

    except (KeyboardInterrupt, SystemExit):
        # Revert the changes from 'curses'
        curses.endwin()
        exit()

wrapper(main)
