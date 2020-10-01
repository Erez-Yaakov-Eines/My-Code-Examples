function DTFT_evaluation( num,den,theta )
% Evaluation of the DTFT 

% Compute the frequency samples of the DTFT
h = freqz(num, den, theta);
% Plot the DTFT

subplot(3,1,2)
plot(theta/pi,abs(h));grid
title('Magnitude Spectrum |H(\theta)|')
xlabel('\theta /\pi');
ylabel('Amplitude');
axis tight;
subplot(3,1,3)
plot(theta/pi,angle(h));grid
title('Phase Spectrum \angle[H(\theta)]')
xlabel('\theta /\pi');
ylabel('Phase in radians');
axis tight;


end

