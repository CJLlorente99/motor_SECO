% Gc = Kp(1+tauD*s)
% G = KM/(s(s+pM))

tauDgrid = 0.1;
tauDmax = 0.01;

figurePerSubplot = 4;
figureNum = 1;

for tauD = [0.003125 0.05]
    num = [tauD*KM KM];
    den = [1 pM 0];
    
%     figure (ceil(figureNum/figurePerSubplot))
    figure(figureNum)
%     subplot (2,2,mod(figureNum-1, figurePerSubplot)+1)
    rlocus(num,den);
    title(['\tau_{d} = ', num2str(tauD)])
    hold on;
    figureNum = figureNum + 1;
    
%     figure (ceil(figureNum/figurePerSubplot))
%     subplot (2,2,mod(figureNum-1, figurePerSubplot)+1)
%     impulse(num,den)
%     title(['Impulso \tau_{d} = ', num2str(tauD)])
%     hold on;
%     figureNum = figureNum + 1;
end

fprintf('createDerivative completed\n')

clear num den figureNum figurePerSubplot tauDmax tauDgrid x