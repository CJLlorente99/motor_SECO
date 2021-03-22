% Gc = Kp(1 + 1/(tauI*s) + tauDs)
% G = KM/(s(s+pM))

tauIgrid = 0.1;
tauImax = 1.2;

tauDgrid = 0.1;
tauDmax = 1.2;

figurePerSubplot = 4;
figureNum = 1;

for tauI = tauIgrid:tauIgrid:tauImax
    for tauD = 0:tauDgrid:tauDmax
        num = [tauD*tauI*KM tauI*KM KM];
        den = [tauI tauI*pM 0 0];

        figure (ceil(figureNum/figurePerSubplot))
        subplot (2,2,mod(figureNum-1, figurePerSubplot)+1)
        rlocus(num,den);
        title(['\tau_{i} = ', num2str(tauI), ' \tau_{d} = ', num2str(tauD)])
        hold on;
        figureNum = figureNum + 1;
    end
end

fprintf('createIntegralDerivative completed\n')

clear num den figureNum figurePerSubplot tauI tauIgrid tauImax tauD tauDgrid tauDmax