function DTFT_freqShift( theta,th0,num1 )
% Frequency-Shifting Properties of DTFT
clf; close all; clc;
i=sqrt(-1);
L = length(num1);
H1 = freqz(num1, 1, theta); 
n = 0:L-1;
num2 = exp(th0*i*n).*num1;
H2 = freqz(num2, 1, theta);
subplot(2,2,1)
plot(theta/pi,abs(H1));grid
title('Magnitude Spectrum of Original Sequence')
xlabel('\theta /\pi');
ylabel('Amplitude');
subplot(2,2,2)
plot(theta/pi,abs(H2));grid
title('Magnitude Spectrum of Frequency-Shifted Sequence')
xlabel('\theta /\pi');
ylabel('Amplitude');
subplot(2,2,3)
plot(theta/pi,angle(H1));grid
title('Phase Spectrum of Original Sequence')
xlabel('\theta /\pi');
ylabel('Phase in radians');
subplot(2,2,4)
plot(theta/pi,angle(H2));grid
title('Phase Spectrum of Frequency-Shifted Sequence')
xlabel('\theta /\pi');
ylabel('Phase in radians');

end

