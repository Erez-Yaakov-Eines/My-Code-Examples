function DTFT_timeShift( theta,D,h1 )
% Time-Shifting Properties of DTFT
clf; close all; clc;
h2 = [zeros(1,D) h1];
figure;
hold on;
subplot(2,1,1);
stem(0:length(h1)-1,h1);
xlabel('Time index n'); ylabel('Amplitude');
title('h_1[n]'); grid;
subplot(2,1,2);
stem(0:length(h2)-1,h2);
xlabel('Time index n'); ylabel('Amplitude');
title('h_2[n] == h_1[n-D]'); grid;
hold off;

figure;
H1 = freqz(h1, 1, theta);
H2 = freqz(h2, 1, theta);
subplot(2,2,1)
plot(theta/pi,abs(H1));grid
title('Magnitude Spectrum of Original Sequence')
xlabel('\theta /\pi');
ylabel('Amplitude');
subplot(2,2,2)
plot(theta/pi,abs(H2));grid
title('Magnitude Spectrum of Time-Shifted Sequence')
xlabel('\theta /\pi');
ylabel('Amplitude');
subplot(2,2,3)
plot(theta/pi,angle(H1));grid
title('Phase Spectrum of Original Sequence')
xlabel('\theta /\pi');
ylabel('Phase in radians');
subplot(2,2,4)
plot(theta/pi,unwrap(angle(H2)));grid
title('Phase Spectrum of Time-Shifted Sequence')
xlabel('\theta /\pi');
ylabel('Phase in radians');
end

