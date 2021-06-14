% Enter your MATLAB code below
readChannelID = 1340875;
readAPIKey = '4WCLTIZ8RR7MR7LT';
[carbonmonoxideData,Timestamps]=thingSpeakRead(1340875,'Fields',[7,8],'NumPoints',5);
plot(Timestamps, carbonmonoxideData(:, 1),'k',Timestamps, carbonmonoxideData(:, 2),'m');
ylabel('Carbon Monoxide Concentration (ppm)');
title('Air Pollution Monitoring');
legend('CO Level', 'Threshold');
