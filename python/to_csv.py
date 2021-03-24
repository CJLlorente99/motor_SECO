import math
import sys
import numpy as np

# if __name__ == "__main__":
#     if sys.argv[1] == '-h' or sys.argv[1] == '--h' or sys.argv[1] == '-help':
#         print('El formato de llamada de este script es el siguiente:', end = '\n')
#         print('py to_csv.py [arrayPositions] (in revolutions)', end = '\n') 
#         print('i.e. py to_csv.py [2, 3, 4] ./to_csv.csv \n')
#         exit()

    # positions = np.asarray(sys.argv[1])
position = 1
    # print(positions)

ContollerType = [0, 1, 2, 3]
TauInit = 2
TauNumber = 4
KpInit = 2
KpNumber = 4

with open("positions.txt", 'w') as myfile:
    myfile.write('const char* csv_str = \t"CONTROLLER, POSITION, TAUI, TAUD, KP\\n"\n')

    m = 0
    for n in range(KpNumber):      
        myfile.write('\t\t\t\t\t\t"' + str(ContollerType[m]) + ", " + str(position*2*math.pi) + ', 0.0, 0.0, ' + str(KpInit/(2**n)) + '\\n" \n')
        # myfile.write('\t\t\t\t\t\t"' + str(ContollerType[m]) + ", " + str(-2*math.pi*(float(positions[i]%1))) + ", 0.0, 0.0, "+ str(n * KpGap + KpGap) + '\\n" \n')
    
    m = 1
    for j in range(TauNumber): 
        for n in range(KpNumber):      
            myfile.write('\t\t\t\t\t\t"' + str(ContollerType[m]) + ", " + str(position*2*math.pi) + ', 0.0, ' + str(TauInit/(2**n)) + ', ' + str(KpInit/(2**n)) + '\\n" \n')
            # myfile.write('\t\t\t\t\t\t"' + str(ContollerType[m]) + ", " + str(-2*math.pi*(float(positions[i]%1))) + ", 0.0, 0.0, "+ str(n * KpGap + KpGap) + '\\n" \n')
    
    m = 2
    for k in range(TauNumber): 
        for n in range(KpNumber):      
            myfile.write('\t\t\t\t\t\t"' + str(ContollerType[m]) + ", " + str(position*2*math.pi) + ', ' + str(TauInit/(2**n)) + ', 0.0, ' + str(KpInit/(2**n)) + '\\n" \n')
            # myfile.write('\t\t\t\t\t\t"' + str(ContollerType[m]) + ", " + str(-2*math.pi*(float(positions[i]%1))) + ", 0.0, 0.0, "+ str(n * KpGap + KpGap) + '\\n" \n')

    m = 3
    for j in range(TauNumber): 
        for k in range(TauNumber): 
            for n in range(KpNumber):      
                myfile.write('\t\t\t\t\t\t"' + str(ContollerType[m]) + ", " + str(position*2*math.pi) + ', ' + str(TauInit/(2**n)) + ', ' + str(TauInit/(2**n)) + ', ' + str(KpInit/(2**n)) + '\\n" \n')
                # myfile.write('\t\t\t\t\t\t"' + str(ContollerType[m]) + ", " + str(-2*math.pi*(float(positions[i]%1))) + ", 0.0, 0.0, "+ str(n * KpGap + KpGap) + '\\n" \n')
    myfile.write('\t\t\t\t\t\t;')

print('CSV file written!', end = '\n')

    