import math
import sys
import numpy as np

if __name__ == "__main__":
    if sys.argv[1] == '-h' or sys.argv[1] == '--h' or sys.argv[1] == '-help':
        print('El formato de llamada de este script es el siguiente:', end = '\n')
        print('py to_csv.py [gap] [repetitions] [time_on] [time_off] [fileName]', end = '\n') 
        print('i.e. py to_csv.py 1 10 1000 1000 ./to_csv.csv \n')
        exit()

    gap = float(sys.argv[1])
    repetitions = int(sys.argv[2])
    time_on = int(sys.argv[3]) # in milisecond
    time_off = int(sys.argv[4]) # in milisecond
    fileName = sys.argv[5]

    voltageValues = np.arange(0, 12+gap, gap)

    with open(fileName, 'w') as myfile:
        myfile.write('"TIME, VOLTAGE\\n"\n')
        for j in range(repetitions):
            for i in voltageValues:        
                myfile.write('"' + str(time_on * 1000) +',' + str(i) + '\\n" \n')
                myfile.write('"' + str(time_off * 1000) +',' + str(0) + '\\n" \n')

    print('CSV file written!', end = '\n')
    print('Estimated test duration: ' + "{:.2f}".format(repetitions*len(voltageValues)*(time_on+time_off)/(1000*60)) + ' minutes', end = '\n')
