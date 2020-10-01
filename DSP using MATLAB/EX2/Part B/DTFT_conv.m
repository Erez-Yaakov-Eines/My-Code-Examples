function DTFT_conv( theta,x1,x2 )
% Convolution Property of DTFT
clf; close all; clc;
% constant definitions
i=sqrt(-1);

% forming the convolved signal.
y = conv(x1,x2);

% plot the discrete signals.
figure;
subplot(3,1,1);
stem(0:length(x1)-1,x1);
title(['x1[n] of length = ' num2str(length(x1))]);
xlabel('Time index n');
ylabel('Amplitude');
subplot(3,1,2);
stem(0:length(x2)-1,x2);
title(['x2[n] of length = ' num2str(length(x2))]);
xlabel('Time index n');
ylabel('Amplitude');
subplot(3,1,3);
stem(0:length(y)-1,y);
title(['x1*x2[n] of length = ' num2str(length(y))]);
xlabel('Time index n');
ylabel('Amplitude');

% calculate the DTFT's
figure;
H1 = freqz(x1, 1, theta);
H2 = freqz(x2, 1, theta);
HP = H1.*H2;
H3 = freqz(y,1,theta);

% Plot element-wise multiplication of manitudes and convolution magnitude
subplot(2,2,1);
plot(theta/pi,abs(HP));grid;
title('Product of Magnitude Spectra');
xlabel('\theta /\pi');
ylabel('Amplitude');
subplot(2,2,2);
plot(theta/pi,abs(H3));grid;
title('Magnitude Spectrum of Convolved Sequence');
xlabel('\theta /\pi');
ylabel('Amplitude');

% plot element-wise sum of phases and convolution phase
subplot(2,2,3);
plot(theta/pi,angle(HP));grid;
title('Sum of Phase Spectra');
xlabel('\theta /\pi');
ylabel('Phase in radians');
subplot(2,2,4);
plot(theta/pi,angle(H3));grid;
title('Phase Spectrum of Convolved Sequence');
xlabel('\theta /\pi');
ylabel('Phase in radians');
end

