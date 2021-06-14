% Enter your MATLAB code below
readChannelID = 1340875;
readAPIKey = '4WCLTIZ8RR7MR7LT';
[hydrogenData,Timestamps]=thingSpeakRead(1340875,'Fields',[3,4],'NumPoints',5);
plot(Timestamps, hydrogenData(:, 1),'k',Timestamps, hydrogenData(:, 2),'m');
ylabel('Hydrogen Concentration(ppm)');
title('Air Pollution Monitoring');
legend('H2 Level', 'Threshold');
