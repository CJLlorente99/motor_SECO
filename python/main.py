import serial
import sys
import numpy as np
import time

if __name__ == "__main__":
    if sys.argv[1] == '-h' or sys.argv[1] == '--h' or sys.argv[1] == '-help':
        print("El formato de llamada a este script es el siguiente \n")
        print("py main.py [SerialPortName] [Baudrate] [path]")
        print("i.e. py main.py COM3 9600 ./textfile.txt")
        
    # receive arguments
    serialPort = sys.argv[1]
    baudrate = int(sys.argv[2])
    path = sys.argv[3]

    # open serial port
    arduinoDue = serial.Serial(serialPort, baudrate=baudrate, timeout=1.0)
    time.sleep(2)

    while True:
        try:
            line = arduinoDue.readline()
            if not line:
                continue
            # line = np.fromstring(line.decode('ascii', errors='replace'), sep=' ')
        except KeyboardInterrupt:
            print("Exiting")
            break

        # Abrir file y escribir CSV en un archivo
        if line:
            line = line.decode('utf-8')
            with open(path, 'at') as file:
                file.write(line + '\n')
                file.close()