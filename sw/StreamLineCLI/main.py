import serial.tools.list_ports
from datetime import datetime
import curses
from curses import wrapper
import time
import re
import os

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

HINTS_WINDOW_HEIGHT = 7
HINTS_WINDOW_WIDTH  = 80
HINTS_WINDOW_X_POS  = 0
HINTS_WINDOW_Y_POS  = 0

DATA_WINDOW_HEIGHT = 15
DATA_WINDOW_WIDTH  = 50
DATA_WINDOW_X_POS  = 0
DATA_WINDOW_Y_POS  = HINTS_WINDOW_HEIGHT

DATA_SEPARATOR = ','
DATA_CARRIAGE_RETURN = '\r'
DATA_LINES = 9

def parse_stream(streamList):

    temp = []
    formated = {}
    for idx, item in enumerate(streamList):
        if idx is DATA_LINES:
            break

        formated_string = item.replace(DATA_SEPARATOR, '')
        formated_string = formated_string.replace(DATA_CARRIAGE_RETURN, '')
        key = formated_string[0:4]
        temp.append(formated_string[4:5])
        temp.append(formated_string[5:])
        formated.update({key: temp.copy()})
        temp.clear()

    return formated

def send_control_output(key, data):

    output = []
    output.append(key)
    output.append(',')

    if data[0] == 'E':
        output.append('D')
    else:
        output.append('E')
    output.append(',')
    output.append(data[1])
    output.append('\r')

    s = ''.join(output)
    b = str.encode(s)
    serialInst.write(b)

def send_control_state(key, data):

    output = []
    output.append(key)
    output.append(',')
    output.append(data[0])
    output.append(',')

    if data[1] == 'OFF':
        output.append('ON')
    elif data[1] == 'ON':
        output.append('OFF')
    output.append('\r')

    s = ''.join(output)
    b = str.encode(s)
    serialInst.write(b)


def data_saver(start_time, data, timestamp):
    # folder where data will be saved locally
    data_folder = './output/'
    # create folder if it doesn't exist
    if os.path.isdir(data_folder) == False:
        os.mkdir(data_folder)

    # filename based on start recording time & type
    filename = datetime.strftime(start_time, '%Y_%m_%d_%H_%M_%S_log')

    file_exists = os.path.exists(data_folder+filename+'.txt')

    if not file_exists:
        file = open(data_folder+filename+'.txt', "x")
        file.close()

    log = open(data_folder+filename+'.txt', "a")

    log.write('\n----------------['+timestamp+']----------------\n')
    for item in data:
        log.write(item)
    log.close()

    return filename

def main(stdscr):

    # Display Stream data content
    data_window = curses.newwin(DATA_WINDOW_HEIGHT, DATA_WINDOW_WIDTH, DATA_WINDOW_Y_POS, DATA_WINDOW_X_POS)

    # Display hints, shortkeys and other info
    hints_window = curses.newwin(HINTS_WINDOW_HEIGHT, HINTS_WINDOW_WIDTH, HINTS_WINDOW_Y_POS, HINTS_WINDOW_X_POS)

    # Create color pairs
    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK)
    curses.init_pair(2, curses.COLOR_RED, curses.COLOR_BLACK)
    curses.init_pair(3, curses.COLOR_YELLOW, curses.COLOR_BLACK)
    GREEN_AND_BLACK = curses.color_pair(1)
    RED_AND_BLACK = curses.color_pair(2)
    YELLOW_AND_BLACK = curses.color_pair(3)

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

    # prog global data
    data_dict = {}
    start_date_and_time = datetime.now()
    current_time = datetime.now()
    current_time = current_time.strftime("%H:%M:%S")

    # Display hints
    hints_window.clear()
    hints_window.addstr('Connected to: {} '.format(portVar))
    hints_window.addstr('with {} baudrate\n'.format(DEFAULT_BAUDRATE))
    hints_window.addstr('Current time: {}\n'.format(current_time))
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
                send_control_output('ADC0', data_dict.get('ADC0'))
            if pressed_key == HINTS_ADC1_OUTPUT_KEY:
                send_control_output('ADC1', data_dict.get('ADC1'))
            if pressed_key == HINTS_LED0_OUTPUT_KEY:
                send_control_output('LED0', data_dict.get('LED0'))
            if pressed_key == HINTS_LED1_OUTPUT_KEY:
                send_control_output('LED1', data_dict.get('LED1'))
            if pressed_key == HINTS_LED2_OUTPUT_KEY:
                send_control_output('LED2', data_dict.get('LED2'))
            if pressed_key == HINTS_LED3_OUTPUT_KEY:
                send_control_output('LED3', data_dict.get('LED3'))
            if pressed_key == HINTS_BTN0_OUTPUT_KEY:
                send_control_output('BTN0', data_dict.get('BTN0'))
            if pressed_key == HINTS_HLD0_OUTPUT_KEY:
                send_control_output('HLD0', data_dict.get('HLD0'))
            if pressed_key == HINTS_TMP0_OUTPUT_KEY:
                send_control_output('TMP0', data_dict.get('TMP0'))
            if pressed_key == HINTS_LED0_STATE_KEY:
                send_control_state('LED0', data_dict.get('LED0'))
            if pressed_key == HINTS_LED1_STATE_KEY:
                send_control_state('LED1', data_dict.get('LED1'))
            if pressed_key == HINTS_LED2_STATE_KEY:
                send_control_state('LED2', data_dict.get('LED2'))
            if pressed_key == HINTS_LED3_STATE_KEY:
                send_control_state('LED3', data_dict.get('LED3'))

            if serialInst.in_waiting:
                packet = serialInst.readline()
                if packet[0] == 0:
                    now = datetime.now()
                    current_time = now.strftime("%H:%M:%S")
                    hints_window.addstr(1, 0, 'Current time: {}\n'.format(current_time))
                    hints_window.refresh()
                    temp_dict = parse_stream(frame)
                    data_dict.update(temp_dict)
                    data_window.clear()
                    for x in range(len(frame)):
                        color_slice = (frame[x])[6:7]
                        if color_slice == 'E':
                            data_window.addstr(x, 0, str(frame[x]), GREEN_AND_BLACK)
                        elif color_slice == 'D':
                            data_window.addstr(x, 0, str(frame[x]), RED_AND_BLACK)
                        else:
                            data_window.addstr(x, 0, str(frame[x]), YELLOW_AND_BLACK)
                        data_window.refresh()
                        # data_print(data_dict, data_window)
                    if len(frame) > 0:
                        data_saver(start_date_and_time, frame, current_time)
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
