% Gc = Kp(1+tauD*s)
% G = KM/(s(s+pM))
reductora = 75;

tauDgrid = 0.01;
tauDmax = 0.02;

figurePerSubplot = 4;
figureNum = 1;

for tauD = 0:tauDgrid:tauDmax
    num = [tauD*KM KM]./reductora;
    den = [1 pM 0];
    
    figure (ceil(figureNum/figurePerSubplot))
    subplot (2,2,mod(figureNum-1, figurePerSubplot)+1)
    rlocus(num,den);
    title(['\tau_{d} = ', num2str(tauD)])
    hold on;
    figureNum = figureNum + 1;
    
    figure (ceil(figureNum/figurePerSubplot))
    subplot (2,2,mod(figureNum-1, figurePerSubplot)+1)
    impulse(num,den)
    title(['Impulso \tau_{d} = ', num2str(tauD)])
    hold on;
    figureNum = figureNum + 1;
end

fprintf('createDerivative completed\n')

clear num den figureNum figurePerSubplot tauDmax tauDgrid x