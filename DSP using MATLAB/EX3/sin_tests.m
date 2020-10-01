clc; clear all; close all;

[tvec,xnTs1]=sample_singen(100,1,8000,pi,0,8000);
[~,xnTs2]=sample_singen(200,1,8000,pi,0,8000);
[~,xnTs3]=sample_singen(400,1,8000,pi,0,8000);
[~,xnTs4]=sample_singen(800,1,8000,pi,0,8000);
[~,xnTs5]=sample_singen(7200,1,8000,pi,0,8000);
[~,xnTs6]=sample_singen(7600,1,8000,pi,0,8000);
[~,xnTs7]=sample_singen(7800,1,8000,pi,0,8000);
[~,xnTs8]=sample_singen(7900,1,8000,pi,0,8000);

figure;



% subplot(2,2,1);
% plot(tvec,xnTs1)
% title('f0=100[Hz]');
% xlabel('t[S]');
% ylabel('Amplitude');
% 
% 
% subplot(2,2,2);
% plot(tvec,xnTs2);
% title('f0=200[Hz]');
% xlabel('t[S]');
% ylabel('Amplitude');
% 
% subplot(2,2,3);
% plot(tvec,xnTs3);
% title('f0=400[Hz]');
% xlabel('t[S]');
% ylabel('Amplitude');
% axis tight;
% 
% subplot(2,2,4);
% plot(tvec,xnTs4);
% title('f0=800[Hz]');
% xlabel('t[S]');
% ylabel('Amplitude');

soundsc([zeros(1,8000) xnTs5 xnTs6 xnTs7 xnTs8]);

% figure;
% 
% subplot(2,2,1);
% plot(tvec,xnTs5)
% title('f0=7200[Hz]');
% xlabel('t[S]');
% ylabel('Amplitude');
% 
% 
% subplot(2,2,2);
% plot(tvec,xnTs6);
% title('f0=7600[Hz]');
% xlabel('t[S]');
% ylabel('Amplitude');
% 
% subplot(2,2,3);
% plot(tvec,xnTs7);
% title('f0=7800[Hz]');
% xlabel('t[S]');
% ylabel('Amplitude');
% axis tight;
% 
% subplot(2,2,4);
% plot(tvec,xnTs8);
% title('f0=7900[Hz]');
% xlabel('t[S]');
% ylabel('Amplitude');
% 
% 
% % bigVec = [xnTs1 xnTs2 xnTs3 xnTs4];
% % soundsc(bigVec,8000);
% 
% 
