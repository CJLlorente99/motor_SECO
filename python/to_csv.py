import math

repetitions = 10
time_on = 10 #In milliseconds
time_off = 10 #In milliseconds
gap = 1

with open('to_csv.csv', 'w') as myfile:
    myfile.write('"TIME, VOLTAGE\\n"\n')
    for i in range(repetitions * (math.floor(12 / gap) + math.floor(math.floor(12/gap) * gap / 12))):
        voltage = math.floor(i/repetitions) * gap
        
        myfile.write('"' + str(time_on * 1000) +',' + str(voltage) + '\\n" \n')
        myfile.write('"' + str(time_off * 1000) +',' + str(0) + '\\n" \n')