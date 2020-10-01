function [tvec,xnTs] = sample_singen(f0,A,fs,phi,n1,n2)
%Sampled Sin wave Generator.
%   [tvec,xnTs] = sample_singen(f0,A,fs,phi,n1,n2) computes a vector of
%   samples from a Sinusoidal of frequency 'f0' with amplitude 'A' and phase
%   'phi'.
%  
% INPUTS:
% 
%   'f0' - Sin frequency.
%   'A' - Sin Amplitude
%   'phi' - Sin phase
%   'fs' - the sampling frequency.
%   'n1' - the first sample discrete time.
%   'n2' - the last sample discrete time.
%
% OUTPUTS:
% 
%   tvec - time vector of length n2-n1+1 containing sample times in seconds.
%   xnTs - sampled vector of length n2-n1+1 containing samples of user
%   specified Sinusoidal at times 'tvec'.
%   

Ts=1/fs; %calc base sample time
tvec=Ts.*(n1:n2); %compute sampling times vector
xnTs=A*sin(2*pi*f0.*tvec+phi); %sample the desired Sinusoidal

end