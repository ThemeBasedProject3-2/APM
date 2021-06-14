% Enter your MATLAB code below
readChannelID = 1407919;
readAPIKey = 'QDDPTUVUQ901MWP4';
[ammoniaData,Timestamps]=thingSpeakRead(1407919,'Fields',[1,2],'NumPoints',300);
plot(Timestamps, ammoniaData(:, 1),'k',Timestamps, ammoniaData(:, 2),'m');
ylabel('Ammonia Concentration (ppm)');
title('Air Pollution Monitoring');
legend('NH3 Level', 'Threshold');
