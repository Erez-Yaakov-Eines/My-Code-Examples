function [ylin,yDFT] = linear_via_circ_conv_less_padd_2(g1,g2)
% Computes the Linear Convolution of signals g1[n] and g2[n]
% directly and by circular convolution, both in
% time and by inverse of DFT multiplication (time circular convolution)
figure;
L1 = length(g1); L2 = length(g2);% claculate series lengths.
g1p = [g1 zeros(1,length(g2)-3)];% 0 padd to L1+L2-2; length of the linear convolution -1
g2p = [g2 zeros(1,length(g1)-3)];% 0 padd to L1+L2-2; length of the linear convolution -1
ylin = circonv(g1p,g2p);% compute the linear convolution using circular convolution.
disp('Linear convolution via circular convolution = ');disp(ylin);% Echo circ conv results.
subplot(3,1,1), stem(0:L1+L2-4,ylin);% plot first part convolved series.
title('linear conv via circular conv g1*g2[n]');
xlabel('n');
ylabel('amplitude');

y = conv(g1, g2);% compute direct linear convolution.
disp('Direct linear convolution = ');disp(y)% Echo linear conv results.
subplot(3,1,2), stem(0:L1+L2-2,y);% plot 2nd part convolved series.
title('direct linear conv g1*g2[n]');
xlabel('n');
ylabel('amplitude');

%using DFT
G1p = fft(g1p); G2p = fft(g2p);% compute DFTs of padded series.
yDFT = real(ifft(G1p.*G2p));% compute real part of inverse of element-wise multiplication
% assumes g1[n] and g2[n] are real.
subplot(3,1,3), stem(0:L1+L2-4,yDFT);% plot results of inverse DFT
title('inverse DFT of {g1[k] times g2[k]}');
xlabel('n');
ylabel('amplitude');

