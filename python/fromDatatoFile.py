import argparse
import pandas as pd
import plotly.express as px
import math
import os

parser = argparse.ArgumentParser(description='From CSV data taken from serial to files for each voltage')
parser.add_argument('inputFile', type=str, help='Path to input CSV file (the one generated from serial)')
parser.add_argument('outputFolder', type=str, help='Folder where output files will be placed (CARE! use / or \\ at the end)')

args = parser.parse_args()

if not os.path.exists(os.path.dirname(args.outputFolder)):
    try:
        os.makedirs(os.path.dirname(args.outputFolder))
    except:
        exit()

if not os.path.exists(os.path.dirname(args.outputFolder + 'data/')):
    try:
        os.makedirs(os.path.dirname(args.outputFolder + 'data/'))
    except:
        exit()

df = pd.read_csv(args.inputFile, skipinitialspace=True, skip_blank_lines=True)

uniqueExperiments = df.groupby(['FINALRAD','CONTROLLER','KP','TAUD','TAUI']).size().reset_index().drop(0,axis=1)
uniqueExperiments = df[['FINALRAD','CONTROLLER','KP','TAUD','TAUI']].drop_duplicates().dropna().reset_index(drop=True)

# Create tags
tags = []
print('Number of unique experiments is : ' + str(uniqueExperiments.shape[0]))
for i in range(uniqueExperiments.shape[0]):
    tags.append('exp_' + str(uniqueExperiments['FINALRAD'][i]) + '_' + str(uniqueExperiments['CONTROLLER'][i]) + '_' + str(uniqueExperiments['KP'][i]) + '_' + str(uniqueExperiments['TAUD'][i]) + '_' + str(uniqueExperiments['TAUI'][i]))
uniqueExperiments['TAG'] = tags

# Summary dile
with open(args.outputFolder + 'summary.txt','w') as file:
    file.write('Los experimentos presentes y sus nombres son\n')
    for i in range(uniqueExperiments.shape[0]):
        file.write('FinalRad = ' + str(uniqueExperiments['FINALRAD'][i]) + ', ' + 'Controller = ' + str(uniqueExperiments['CONTROLLER'][i]) + ', ' \
        + ', ' + 'Kp = ' + str(uniqueExperiments['KP'][i]) + ', ' + 'TauD = ' + str(uniqueExperiments['TAUD'][i]) + ', ' + 'TauI = ' + str(uniqueExperiments['TAUI'][i]) + ', ' \
        + 'nombre del fichero = ' + args.outputFolder + 'data/' + uniqueExperiments['TAG'][i] + '.txt\n')

# For each unique experiment create file with data and plot
for i in range(uniqueExperiments.shape[0]):
    data = df.loc[(df['FINALRAD'] == uniqueExperiments['FINALRAD'][i]) & (df['CONTROLLER'] == uniqueExperiments['CONTROLLER'][i]) & (df['KP'] == uniqueExperiments['KP'][i]) & (df['TAUD'] == uniqueExperiments['TAUD'][i]) & (df['TAUI'] == uniqueExperiments['TAUI'][i])].reset_index(drop=True)
    with open(args.outputFolder + 'data/' + uniqueExperiments['TAG'][i] + '.txt','w') as file:
        file.write('TIME,PULSES,CONTROLLERVOLTAGE,CONTROLLER,KP,TAUD,TAUI \n')
        for j in range(data.shape[0]):
            file.write(str(data['TIME'][j]) + ',' + str(data['PULSES'][j]) + ',' + str(data['CONTROLLERVOLTAGE'][j]) + ',' + str(data['CONTROLLER'][j]) + ',' + str(data['KP'][j]) + ',' + str(data['TAUD'][j]) + ',' + str(data['TAUI'][j]) + '\n')

    fig = px.scatter(x=data['TIME'], y=(data['PULSES']*2*math.pi/48/75), title=uniqueExperiments['TAG'][i], labels=dict(x='Time [ms]', y='Position [rad]'))
    fig.add_hline(y=uniqueExperiments['FINALRAD'][i], line_color='red', line_width=1, line_dash='dash')

    fig.write_html(args.outputFolder + 'data/' + uniqueExperiments['TAG'].loc[i] + '.html')    
    print('Experiment number : ' + str(i+1) + ' processed')

print("Execution finished")