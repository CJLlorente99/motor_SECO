import serial
import sys
import numpy as np
import time

if __name__ == "__main__":
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
            line = line.decode('ascii')
            with open(path, 'at') as file:
                file.write(line + '\n')
                file.close()