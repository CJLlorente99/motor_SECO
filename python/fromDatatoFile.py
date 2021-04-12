import argparse
import pandas as pd
import plotly.express as px
import math
import os

parser = argparse.ArgumentParser(description='From CSV data taken from serial to files for each experiment')
parser.add_argument('inputFolder', type=str, help='Path to input folder containing CSV files (the one generated from serial)')

args = parser.parse_args()

if not os.path.exists(os.path.dirname('./outputData')):
    try:
        os.makedirs(os.path.dirname('./outputData'))
    except:
        exit()

inputFiles = [f for f in os.listdir(args.inputFolder) if os.path.isfile(os.path.join(args.inputFolder, f))]
for i in range(len(inputFiles)):
    if not os.path.exists(os.path.dirname('./outputData/data' + str(i+1) + '/')):
        try:
            os.makedirs(os.path.dirname('./outputData/data' + str(i+1) + '/'))
        except:
            exit()

    if not os.path.exists(os.path.dirname('./outputData/data' + str(i+1) + '/data/')):
        try:
            os.makedirs(os.path.dirname('./outputData/data' + str(i+1) + '/data/'))
        except:
            exit() 

for fileName in inputFiles:
    fileName = args.inputFolder + fileName
    idx = fileName[-5]

    df = pd.read_csv(fileName, skipinitialspace=True, skip_blank_lines=True)

    uniqueExperiments = df.groupby(['FINALRAD','CONTROLLER','KP','TAUD','TAUI']).size().reset_index().drop(0,axis=1)
    uniqueExperiments = df[['FINALRAD','CONTROLLER','KP','TAUD','TAUI']].drop_duplicates().dropna().reset_index(drop=True)

    # Create tags
    tags = []
    print('Number of unique experiments is : ' + str(uniqueExperiments.shape[0]))
    for i in range(uniqueExperiments.shape[0]):
        tags.append('exp_' + str(uniqueExperiments['FINALRAD'][i]) + '_' + str(uniqueExperiments['CONTROLLER'][i]) + '_' + str(uniqueExperiments['KP'][i]) + '_' + str(uniqueExperiments['TAUD'][i]) + '_' + str(uniqueExperiments['TAUI'][i]))
    uniqueExperiments['TAG'] = tags

    # Summary file
    with open('./outputData/data' + str(idx) + '/summary.txt','w') as file:
        file.write('Los experimentos presentes y sus nombres son\n')
        for i in range(uniqueExperiments.shape[0]):
            file.write('FinalRad = ' + '{0:.4f}'.format(uniqueExperiments['FINALRAD'][i]) + ', ' + 'Controller = ' + str(uniqueExperiments['CONTROLLER'][i]) + ', ' \
            + ', ' + 'Kp = ' + '{0:.4f}'.format(uniqueExperiments['KP'][i]) + ', ' + 'TauD = ' + '{0:.4f}'.format(uniqueExperiments['TAUD'][i]) + ', ' + 'TauI = ' + '{0:.4f}'.format(uniqueExperiments['TAUI'][i]) + ', ' \
            + 'nombre del fichero = ' + 'outputFolder/data' + str(idx) + '/data/' + uniqueExperiments['TAG'][i] + '.txt\n')

    # For each unique experiment create file with data and plot
    for i in range(uniqueExperiments.shape[0]):
        data = df.loc[(df['FINALRAD'] == uniqueExperiments['FINALRAD'][i]) & (df['CONTROLLER'] == uniqueExperiments['CONTROLLER'][i]) & (df['KP'] == uniqueExperiments['KP'][i]) & (df['TAUD'] == uniqueExperiments['TAUD'][i]) & (df['TAUI'] == uniqueExperiments['TAUI'][i])].reset_index(drop=True)
        with open('./outputData/data' + str(idx) + '/data/' + uniqueExperiments['TAG'][i] + '.txt','w') as file:
            file.write('TIME,PULSES,CONTROLLERVOLTAGE,CONTROLLER,KP,TAUD,TAUI \n')
            for j in range(data.shape[0]):
                file.write(str(data['TIME'][j]) + ',' + str(data['PULSES'][j]) + ',' + str(data['CONTROLLERVOLTAGE'][j]) + ',' + str(data['CONTROLLER'][j]) + ',' + str(data['KP'][j]) + ',' + str(data['TAUD'][j]) + ',' + str(data['TAUI'][j]) + '\n')

        fig = px.scatter(x=data['TIME'], y=(data['PULSES']*2*math.pi/48/75), title=uniqueExperiments['TAG'][i], labels=dict(x='Time [ms]', y='Position [rad]'))
        fig.add_hline(y=uniqueExperiments['FINALRAD'][i], line_color='red', line_width=1, line_dash='dash')

        fig.write_html('./outputData/data' + str(idx) + '/data/' + uniqueExperiments['TAG'].loc[i] + '.html')
        fig.write_image('./outputData/data' + str(idx) + '/data/' + uniqueExperiments['TAG'].loc[i] + '.svg')     
        print('Experiment number : ' + str(i+1) + ' processed')

    print('Finished dealing with ' + str(fileName))

print("Execution finished")