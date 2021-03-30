import argparse
import pandas as pd

parser = argparse.ArgumentParser(description='From CSV data taken from serial to files for each voltage')
parser.add_argument('inputFile', type=str, help='Path to input CSV file (the one generated from serial)')
parser.add_argument('outputFolder', type=str, help='Folder where output files will be placed (CARE! use / or \\ at the end)')

args = parser.parse_args()

df = pd.read_csv(args.inputFile)

voltageValues = df['VOLTAGE'].unique()
meanValues = pd.DataFrame(columns=voltageValues, index=range(1201))

for voltage in voltageValues:
    print('\nDealing with voltage ' + str(voltage))
    print('This voltage has ' + str(df[(df['VOLTAGE'] == voltage) & (df[' INDEX'] == 0)][' PULSES'].count()) + ' repetitions')
    for i in range(1201):
        meanValues[voltage][i] = df[(df['VOLTAGE'] == voltage) & (df[' INDEX'] == i)][' PULSES'].mean()
    print('Dealt with voltage ' + str(voltage) + '\n')

for voltage in voltageValues:
    if(voltage % 1 == 0):
        fileName ='trap' + str(int(voltage)) + 'V_0ms600ms600ms_T1ms_ST.mean.'
    else:
        fileName ='trap' + str(voltage) + 'V_0ms600ms600ms_T1ms_ST.mean.'

    print('Starting to write ' + fileName) 
    with open(args.outputFolder + fileName, 'w') as f:
        for i in range(1201):
            f.write(str(i) + ' ' + str(meanValues[voltage][i]*75) + "\n")
    print('Finished writing ' + fileName + '\n')

