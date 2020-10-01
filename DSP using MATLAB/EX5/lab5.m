%% EX5, part 1


clear all; close all; clc;
N=2^8;
n=0:N-1;
x_n= cos(2*pi*0.25*n)+0.01*sin(2*pi*0.1*n);

rect_win=window(@rectwin,N).';
bartlett_win=window(@bartlett,N).';
hamming_win=window(@hamming,N).';
hann_win=window(@hann,N).';
blackman_win=window(@blackman,N).';

x_n_rect=x_n.*rect_win;
x_n_bart=x_n.*bartlett_win;
x_n_hamm=x_n.*hamming_win;
x_n_hann=x_n.*hann_win;
x_n_black=x_n.*blackman_win;

% figure('Name','rectangular');
% freqz(x_n_rect);
% 
% figure('Name','bartlett');
% freqz(x_n_bart);
% 
% figure('Name','hamming');
% freqz(x_n_hamm);
% 
% figure('Name','hann');
% freqz(x_n_hann);
% 
% figure('Name','blackman');
% freqz(x_n_black);

figure('Name','rectangular-window-only');
freqz(rect_win./256);

figure('Name','bartlett-window-only');
freqz(bartlett_win./127.498);

figure('Name','hann-window-only');
freqz(hann_win./127.5);

figure('Name','hamming-window-only');
freqz(hamming_win./137.78);

figure('Name','blackman-window-only');
freqz(blackman_win./107.1);


%% EX5, part 2

N1=2^8;
N2=2^10;
n1=0:N1-1;
n2=0:N2-1;
theta_1=pi/2+pi/3;
theta_2=pi/2+7*pi/256;
theta_3=pi/2+3*pi/256;

theta_4=pi/2+pi/3;
theta_5=pi/2+8*pi/1024;
theta_6=pi/2+3*pi/1024;

x_n_1= cos(2*pi*0.25*n1)+0.01*sin(theta_1*n1);
x_n_2= cos(2*pi*0.25*n1)+0.01*sin(theta_2*n1);
x_n_3= cos(2*pi*0.25*n1)+0.01*sin(theta_3*n1);

x_n_4= cos(2*pi*0.25*n2)+0.01*sin(theta_4*n2);
x_n_5= cos(2*pi*0.25*n2)+0.01*sin(theta_5*n2);
x_n_6= cos(2*pi*0.25*n2)+0.01*sin(theta_6*n2);

blackman_win_N1=window(@blackman,N1).';
blackman_win_N2=window(@blackman,N2).';


x_n_black_1=x_n_1.*blackman_win_N1;
x_n_black_2=x_n_2.*blackman_win_N1;
x_n_black_3=x_n_3.*blackman_win_N1;

x_n_black_4=x_n_4.*blackman_win_N2;
x_n_black_5=x_n_5.*blackman_win_N2;
x_n_black_6=x_n_6.*blackman_win_N2;
% 
% figure('Name','/theta before');
% freqz(x_n_black_1);
% figure('Name','/theta while');
% freqz(x_n_black_2);
% figure('Name','/theta after');
% freqz(x_n_black_3);
% 
% figure('Name','/theta before');
% freqz(x_n_black_4);
% figure('Name','/theta while');
% freqz(x_n_black_5);
% figure('Name','/theta after');
% freqz(x_n_black_6);

