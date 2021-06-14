% Enter your MATLAB code below
readChannelID = 1407919;
readAPIKey = 'QDDPTUVUQ901MWP4';
[carbondioxideData,Timestamps]=thingSpeakRead(1407919,'Fields',[5,6],'NumPoints',300);
plot(Timestamps, carbondioxideData(:, 1),'k',Timestamps, carbondioxideData(:, 2),'m');
ylabel('Carbon dioxide Concentration (ppm)');
title('Air Pollution Monitoring');
legend('CO2 Level', 'Threshold');
