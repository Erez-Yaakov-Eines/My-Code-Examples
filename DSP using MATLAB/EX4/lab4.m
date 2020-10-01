%% EX1, 1
x_n=[ones(1,4) 0 0 0 0];
x_k=fft(x_n);
length(x_k)
%% EX1, 2
y_n=ifft(x_k);
diff_xy=y_n-x_n;
%% EX1, 3
x_k_4=fft(x_n,4);
x_k_6=fft(x_n,6);
x_k_10=fft(x_n,10);
x_k_16=fft(x_n,16);
x_n_4=ifft(x_k_4,4);
x_n_6=ifft(x_k_6,6);
x_n_10=ifft(x_k_10,10);
x_n_16=ifft(x_k_16,16);

figure;
subplot(4,2,1);
stem(0:3,x_n_4);
title('x_n_4');
subplot(4,2,2);
stem(0:3,abs(x_k_4),'red');
title('x_k_4');
subplot(4,2,3);
stem(0:5,x_n_6);
title('x_n_6');
subplot(4,2,4);
stem(0:5,abs(x_k_6),'red');
title('x_k_6');
subplot(4,2,5);
stem(0:9,x_n_10);
title('x_n_10');
subplot(4,2,6);
stem(0:9,abs(x_k_10),'red');
title('x_k_10');
subplot(4,2,7);
stem(0:15,x_n_16);
title('x_n_16');
subplot(4,2,8);
stem(0:15,abs(x_k_16),'red');
title('x_k_16');

%% EX2,2
g1=[1 2 3 4 5 6]; g2=[1 -2 3 3 -2 1];
g3=[1 1 1 1];
circ_property_DFT(g1,g2);
circ_property_DFT(g3,g3);

%% EX3,2
g1=[1 2 3 4 5 6]; g2=[10 11 12 13 14 15];
[ylin,yDFT]=linear_via_circ_conv(g1,g2);

%% EX3,3
figure;
stem(0:length(g1)+length(g2)-2,ylin-yDFT);
title('ylin-yDFT');
xlabel('n');
ylabel('Amplitude');
axis tight;

%% EX3,4
linear_via_circ_conv_less_padd_1(g1,g2);

%% EX3,5
linear_via_circ_conv_less_padd_2(g1,g2);

%% EX4, 3
L=8;
n=0:4*L-1;
x_n=cos(2*pi*n./8);

figure;
subplot(6,2,1);
stem(0:2*L-1,x_n(1:1:2*L));
title('x[n] 2L')
xlabel('n');
ylabel('Amplitude');
axis tight;
subplot(6,2,2);
stem(0:2*L-1,abs(fft(x_n,2*L)),'red');
title('x^d[k] 2L');
xlabel('k');
ylabel('Amplitude');
axis tight;

subplot(6,2,3);
stem(0:3*L-1,x_n(1:1:3*L));
title('x[n] 3L')
xlabel('n');
ylabel('Amplitude');
axis tight;
subplot(6,2,4);
stem(0:3*L-1,abs(fft(x_n,3*L)),'red');
title('x^d[k] 3L');
xlabel('k');
ylabel('Amplitude');
axis tight;

subplot(6,2,5);
stem(0:3.25*L-1,x_n(1:1:3.25*L));
title('x[n] 3.25L')
xlabel('n');
ylabel('Amplitude');
axis tight;
subplot(6,2,6);
stem(0:3.25*L-1,abs(fft(x_n,3.25*L)),'red');
title('x^d[k] 3.25L');
xlabel('k');
ylabel('Amplitude');
axis tight;

subplot(6,2,7);
stem(0:3.5*L-1,x_n(1:1:3.5*L));
title('x[n] 3.5L')
xlabel('n');
ylabel('Amplitude');
axis tight;
subplot(6,2,8);
stem(0:3.5*L-1,abs(fft(x_n,3.5*L)),'red');
title('x^d[k] 3.5L');
xlabel('k');
ylabel('Amplitude');
axis tight;

subplot(6,2,9);
stem(0:3.75*L-1,x_n(1:1:3.75*L));
title('x[n] 3.75L')
xlabel('n');
ylabel('Amplitude');
axis tight;
subplot(6,2,10);
stem(0:3.75*L-1,abs(fft(x_n,3.75*L)),'red');
title('x^d[k] 3.75L');
xlabel('k');
ylabel('Amplitude');
axis tight;

subplot(6,2,11);
stem(0:4*L-1,x_n(1:1:4*L));
title('x[n] 4L')
xlabel('n');
ylabel('Amplitude');
axis tight;
subplot(6,2,12);
stem(0:4*L-1,abs(fft(x_n,4*L)),'red');
title('x^d[k] 4L');
xlabel('k');
ylabel('Amplitude');
axis tight;

%% EX4, 4
Ts=1/100;
fs=1/Ts;
x_k=fft(x_n);
x_k_reorder=[x_k(2*L+1:length(x_k)) x_k(1:2*L) 0];
w=(-2*L:2*L).*(pi*fs/16);

figure;
stem(w,abs(x_k_reorder));
title('X^F(j\omega)');
xlabel('$\displaystyle \omega [\frac{rad}{sec}]$','interpreter','latex');
ylabel('Relative Amplitude');
axis tight;