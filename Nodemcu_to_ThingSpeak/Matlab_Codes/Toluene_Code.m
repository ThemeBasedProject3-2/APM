% Enter your MATLAB code below
readChannelID = 1407919;
readAPIKey = 'QDDPTUVUQ901MWP4';
[tolueneData,Timestamps]=thingSpeakRead(1407919,'Fields',[3,4],'NumPoints',300);
plot(Timestamps, tolueneData(:, 1),'k',Timestamps, tolueneData(:, 2),'m');
ylabel('Toluene Concentration (ppm)');
title('Air Pollution Monitoring');
legend('Toluene Level', 'Threshold');
