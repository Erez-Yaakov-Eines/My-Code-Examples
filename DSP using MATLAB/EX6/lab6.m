th0=2*pi*0.2;
j=sqrt(-1);

%%EX1, FIR 1

%{
num=[1 0.5 0.5 0.5 1];
den=[1];
figure;
zplane(num,den);grid on;

N = 512;
theta=linspace(-pi,pi,N);
[H,~]=freqz(num,den,theta);

figure;
subplot(2,1,1);
plot(theta./pi,abs(H));
title('FIR 1 Amplitude');
xlabel('angle [rad/\pi]');
ylabel('Amplitude');
axis tight;
subplot(2,1,2);
plot(theta./pi,angle(H));
title('FIR 1 Phase');
xlabel('angle [rad/\pi]');
ylabel('Phase Angle [rad]');
axis tight;

N_sin = 256;
n = 0:N_sin-1;
x_n = sin(th0.*n);
y_n = ifft(fft(x_n,N_sin).*fft(num,N_sin));

figure; 
A_theta=2*cos(2*th0)+cos(th0)+0.5;
subplot(2,1,1); stem(n,x_n);title('x[n] = sin(2*pi*0.2.*n)');
axis([100 150 min(x_n) max(x_n)]);
subplot(2,1,2); stem(n,(y_n/A_theta));title('y[n] = x[n-{\tau}_g]'); 
axis([100 150 min(y_n/A_theta) max(y_n/A_theta)]);
%}
%{
%EX1, FIR 2


num=[1 1 1 1];
den=[1];
figure;
zplane(num,den);grid on;

N = 512;
theta=linspace(-pi,pi,N);
[H,~]=freqz(num,den,theta);

figure;
subplot(2,1,1);
plot(theta./pi,abs(H));
title('FIR 2 Amplitude');
xlabel('angle [rad/\pi]');
ylabel('Amplitude');
axis tight;
subplot(2,1,2);
plot(theta./pi,angle(H));
title('FIR 2 Phase');
xlabel('angle [rad/\pi]');
ylabel('Phase Angle [rad]');
axis tight;

N_sin = 256;
n = 0:N_sin-1;
x_n = sin(th0.*n);
y_n = ifft(fft(x_n,N_sin).*fft(num,N_sin));

figure; 
A_theta=2*cos(1.5*th0)+2*cos(0.5*th0);
subplot(2,1,1); stem(n,x_n);title('x[n] = sin(2*pi*0.2.*n)');
axis([100 150 min(x_n) max(x_n)]);
subplot(2,1,2); stem(n,(y_n/A_theta));title('y[n] = x[n-{\tau}_g]'); 
axis([100 150 min(y_n/A_theta) max(y_n/A_theta)]);
%}

%EX1, FIR 3

%{
num=[1 1 0 -1 -1];
den=[1];
figure;
zplane(num,den);grid on;

N = 512;
theta=linspace(-pi,pi,N);
[H,~]=freqz(num,den,theta);

figure;
subplot(2,1,1);
plot(theta./pi,abs(H));
title('FIR 3 Amplitude');
xlabel('angle [rad/\pi]');
ylabel('Amplitude');
axis tight;
subplot(2,1,2);
plot(theta./pi,angle(H));
title('FIR 3 Phase');
xlabel('angle [rad/\pi]');
ylabel('Phase Angle [rad]');
axis tight;

N_sin = 256;
n = 0:N_sin-1;
x_n = sin(th0.*n);
y_n = ifft(fft(x_n,N_sin).*fft(num,N_sin));

figure; 
A_theta=2*sin(2*th0)+2*sin(th0);
subplot(2,1,1); stem(n,x_n);title('x[n] = sin(2*pi*0.2.*n)');
axis([100 150 min(x_n) max(x_n)]);
subplot(2,1,2); stem(n,(y_n/A_theta));title('y[n] = x[n-{\tau}_g]'); 
axis([100 150 min(y_n/A_theta) max(y_n/A_theta)]);
%}

%EX1, FIR 4

num=[1 1 1 -1 -1];
den=[1];
figure;
zplane(num,den);grid on;

N = 512;
theta=linspace(-pi,pi,N);
[H,~]=freqz(num,den,theta);

figure;
subplot(2,1,1);
plot(theta./pi,abs(H));
title('FIR 4 Amplitude');
xlabel('angle [rad/\pi]');
ylabel('Amplitude');
axis tight;
subplot(2,1,2);
plot(theta./pi,angle(H));
title('FIR 4 Phase');
xlabel('angle [rad/\pi]');
ylabel('Phase Angle [rad]');
axis tight;

N_sin = 256;
n = 0:N_sin-1;
x_n = sin(th0.*n);
y_n = ifft(fft(x_n,N_sin).*fft(num,N_sin));

figure; 
subplot(2,1,1); stem(n,x_n);title('x[n] = sin(2*pi*0.2.*n)');
axis([100 150 min(x_n) max(x_n)]);
subplot(2,1,2); stem(n,(y_n));title('y[n] = x[n-{\tau}_g]'); 
axis([100 150 min(y_n) max(y_n)]);

%%EX2, AP
%{
figure;

p=[0.5*exp(j*pi/4) 0.5*exp(-j*pi/4)].';
z=[2*exp(j*pi/4) 2*exp(-j*pi/4)].';
zplane(z,p); grid on;

k=0.25;
[b,a]= zp2tf(z,p,k)

N = 512;
theta=linspace(-pi,pi,N);
[H,~]=freqz(b,a,theta);

figure;
subplot(2,1,1);
plot(theta./pi,abs(H));
title('AllPass Amplitude');
xlabel('angle [rad/\pi]');
ylabel('Amplitude');
axis tight;
subplot(2,1,2);
plot(theta./pi,angle(H));
title('AllPass Phase');
xlabel('angle [rad/\pi]');
ylabel('Phase Angle [rad]');
axis tight;
%}

%%EX3, minumum phase
%{
figure;
p=[0.25 0.2].';
z=[1/3 4].';
zplane(z,p); grid on;

k=1;
[b,a]= zp2tf(z,p,k);

N = 512;
theta=linspace(-pi,pi,N);
[H,~]=freqz(b,a,theta);

k_ap=4;
z_ap=0.25;
p_ap=4;
[b_ap,a_ap]=zp2tf(z_ap,p_ap,k_ap);
[Hap,~]=freqz(b_ap,a_ap,theta);

b_g=conv(b,b_ap);
a_g=conv(a,a_ap);

figure;
subplot(2,3,1);
plot(theta,abs(H));
title('|H^f(\theta)|');
xlabel('\theta'); 
axis tight;
subplot(2,3,2); 
plot(theta,abs(Hap)); 
title('|H^f_a_p(\theta)|'); 
xlabel('\theta'); 
axis tight;
subplot(2,3,3); 
plot(theta,abs(G)); 
title('|G^f(\theta)|');
xlabel('\theta'); 
axis tight;
subplot(2,3,4); 
plot(theta,angle(H)); 
title('angle(H)');
xlabel('\theta'); 
axis tight;
subplot(2,3,5); 
plot(theta,angle(Hap)); 
title('angle(H^f_a_p)');
xlabel('\theta'); 
axis tight;
subplot(2,3,6); 
plot(theta,angle(G)); 
title('angle(G)');
xlabel('\theta'); 
axis tight;
%}