function [tvec,cnTs] = sample_chirpgen(f1,A,mu,phi,fs,n1,n2)
%Samples a chirp signal of linear-varying frequency.
% 
% [tvec,cnTs] = sample_chirpgen(f1,A,mu,phi,fs,n1,n2) Samples the chirp
% signal of base frequency f1, at fs Hz
% 
% INPUTS:
%   f1 = chirp base frequency.
%   A = chirp Amplitude.
%   mu = frequency variation in [Hz/sec] or [1/sec^2].
%   phi = offset Phase.
%   fs = sampling frequency.
%   n1 = first sample discrete time.
%   n2 = last sample discrete time.
% 
% OUTPUTS:
%   tvec = time vector of length n2-n1+1 containing sample times in seconds.
%   cnTs = sampled vector of length n2-n1+1 containing samples of user
%   specified Chirp at times 'tvec'.
%   

Ts=1/fs; %calc base sample time
tvec=Ts.*(n1:n2); %compute time vector
cnTs=A.*cos(pi*mu*(tvec).^2+2*pi*f1.*tvec+phi); %compute Chirp samples vector
end

