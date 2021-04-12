% Gc = Kp(1 + 1/(tauI*s) + tauDs)
% G = KM/(s(s+pM))

reductora = 75;

tauIgrid = 100;
tauImax = 1000;

tauDgrid = 0.1;
tauDmax = 1.2;

figurePerSubplot = 4;
figureNum = 1;

for tauI = [1 0.25]
    for tauD = [0.01 0.0025]
        num = [tauD*tauI*KM tauI*KM KM];
        den = [tauI tauI*pM 0 0];

%         figure (ceil(figureNum/figurePerSubplot))
%         subplot (2,2,mod(figureNum-1, figurePerSubplot)+1)
        figure(figureNum)
        rlocus(num,den);
        title(['\tau_{i} = ', num2str(tauI), ' \tau_{d} = ', num2str(tauD)])
        hold on;
        figureNum = figureNum + 1;
        
%         figure (ceil(figureNum/figurePerSubplot))
%         subplot (2,2,mod(figureNum-1, figurePerSubplot)+1)
%         impulse(num,den)
%         title(['Impulso \tau_{i} = ', num2str(tauI), ' \tau_{d} = ', num2str(tauD)])
%         hold on;
%         figureNum = figureNum + 1;
    end
end

fprintf('createIntegralDerivative completed\n')

clear num den figureNum figurePerSubplot tauI tauIgrid tauImax tauD tauDgrid tauDmax