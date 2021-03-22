% Gc = Kp(1+1/(tauI*s))
% G = KM/(s(s+pM))

tauIgrid = 0.05;
tauImax = 2;

figurePerSubplot = 4;
figureNum = 1;

for tauI = tauIgrid:tauIgrid:tauImax
    num = [tauI*KM KM];
    den = [tauI pM*tauI 0 0];
    
    figure (ceil(figureNum/figurePerSubplot))
    subplot (2,2,mod(figureNum-1, figurePerSubplot)+1)
    rlocus(num,den);
    title(['\tau_{i} = ', num2str(tauI)])
    hold on;
    figureNum = figureNum + 1;
end

fprintf('createIntegral completed\n')

clear num den figureNum figurePerSubplot tauI tauIgrid tauImax