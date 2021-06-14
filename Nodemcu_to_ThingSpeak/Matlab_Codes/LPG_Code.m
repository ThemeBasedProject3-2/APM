% Enter your MATLAB code below
readChannelID = 1340875;
readAPIKey = '4WCLTIZ8RR7MR7LT';
[lpgData,Timestamps]=thingSpeakRead(1340875,'Fields',[5,6],'NumPoints',5);
plot(Timestamps, lpgData(:, 1),'k',Timestamps, lpgData(:, 2),'m');
ylabel('LPG Concentration (ppm)');
title('Air Pollution Monitoring');
legend('LPG Level', 'Threshold');
