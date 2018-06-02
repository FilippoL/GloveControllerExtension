import sys
import glob
import serial
import time

def singleton(cls, *args, **kw):
    instances = {}
    def _singleton():
       if cls not in instances:
            instances[cls] = cls(*args, **kw)
       return instances[cls]
    return _singleton

@singleton
class MySerializer:
    def __init__(self, _baudrate=9600, _timeout=100):
        self._ser = serial.Serial()
        self._ser.baudrate = _baudrate
        self._ser.timeout = _timeout

    def open(self):
        ''' Open the serial port.'''
        self._ser.open()

    def close(self):
        ''' Close the serial port.'''
        self._ser.close()

    def send(self, msg):
        self._ser.write(msg)

    def recv(self):
        return self._ser.readline()

    def getName(self):
        return str(self._ser.name)

    def connectionAvailable(self):
        return self._ser.isOpen()

    def SetPort(self, value):
        self._ser.port = value


def checkSystemPorts():
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')
    return ports


def Serial_connect(_br):
    ser = MySerializer()
    for port in checkSystemPorts():
        try:
            ser.SetPort(port)
            ser.open()
            return "True"
        except (OSError, serial.SerialException):
            pass


    if not ser.connectionAvailable():
        print("No ports available, retrying...")
        return "False"

def Release():
    ser = MySerializer()
    ser.close()
    print("Serial closed")

def GetVars():
    ser = MySerializer()
    checking = 1
    lines = []


    while checking:
        lines.append(ser.recv())
        if lines[0] == "BEGIN \n":
            checking = 0
        lines = []

    for value in range(0, 9):
        lines.append(ser.recv())

        if lines[0] == "SHUTDOWN\n":
            ProgramOn = 0

        if lines[0] == "STOP\n":
            ProgramOn = 0


    return str(lines).strip('[]')
