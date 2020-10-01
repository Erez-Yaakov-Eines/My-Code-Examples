function circ_property_DFT(g1,g2)
% investigates the Circular Convolution Property of DFT
% by comparing the circular convolution computed directly of two dicrete 
% signals g1 and g2, to the inverse transform of their frequency domain
% element-wise multiplication.
ycir = circonv(g1,g2);% calculate the circular convolution of discrete signals
% g1 and g2.
disp('Result of circular convolution = ');disp(ycir)% Echos info string
% and result of circular convolution.
G1 = fft(g1); G2 = fft(g2);% calculate DFTs of g1 and g2.
yc = real(ifft(G1.*G2));% compute real part of the inverse DFT of element-
% wise multiplication of DFTs (assumes g1[n] and g2[n] are real signals.
disp('Result of IDFT of the DFT products = ');% Echos string to user
disp(yc)% Echos calculated inverse of DFT multiplication of g1 and g2.
