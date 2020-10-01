clear all; close all; clc;
K=10^3;
f1=100;
A=1;
mu=40000;
phi=0;
fs=8*K;
n1=0;
n2=10*fs;
[tvec,cnTs]=sample_chirpgen(f1,A,mu,phi,fs,n1,n2);
figure;
stem(tvec(1:2000),cnTs(1:2000));
title('first 2000 samples (1/8 sec) of the Chirp signal:   c(nT) = Acos(\pi\mu(nT_s)^2+2\pif_1(nT_s)+\phi)');
xlabel('time[sec]');
ylabel('Amplitude');
axis tight;
soundsc(cnTs,fs);