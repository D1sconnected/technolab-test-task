import serial.tools.list_ports
from datetime import datetime

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()

portList = []

for onePort in ports:
    portList.append(str(onePort))
    print(str(onePort))

val = input("Enter COM port number to connect: ")

for x in range(0,len(portList)):
    if portList[x].startswith("COM" + str(val)):
        portVar = "COM" + str(val)
        print(portList[x])

# Create list
dataDict = {}
tempList = []

now = datetime.now()
current_time = now.strftime("%H:%M:%S")

serialInst.baudrate = 115200
serialInst.port = portVar
serialInst.open()

while True:
    if serialInst.in_waiting:
        packet = serialInst.readline()
        if packet[0] == 0:
            now = datetime.now()
            current_time = now.strftime("%H:%M:%S")
            tempDict = {current_time: tempList}
            dataDict.update(tempDict)
            continue
        tempList.append(packet.decode('utf').rstrip('\n'))
        print(packet.decode('utf').rstrip('\n'))
