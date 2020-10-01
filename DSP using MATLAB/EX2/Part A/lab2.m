%% EX2,a
clf;
N = 40;
n=0:N-1;
u = ones(1,N);


% Compute the impulse response y
b = [1];
a = [1 -0.35];
y = impz(b,a,N);
% Plot the impulse response
subplot(3,1,1);
stem(n,y);
xlabel('Time index n'); ylabel('Amplitude');
title('y[n]-0.35y[n-1]=x[n]'); grid;
% Compute the Step response y
s = filter(b,a,u);
hold on
% Plot the Step response
stem(n,s);
legend('Impulse Response','Step Response');
hold off;


%% EX2,b
figure
% Compute the impulse response y
b = [1 -0.25];
a = [1 -0.75];
y = impz(b,a,N);
% Plot the impulse response
subplot(3,1,1);
stem(n,y);
xlabel('Time index n'); ylabel('Amplitude');
title('y[n]-0.75y[n-1]=x[n]-0.25x[n-1]'); grid;
hold on
% Compute the Step response y
s = filter(b,a,u);
% Plot the Step response
stem(n,s);
legend('Impulse Response','Step Response');
hold off;

%% EX2,c
figure
% Compute the impulse response y
b = [1];
a = [1 -0.75 -0.25];
y = impz(b,a,N);
% Plot the impulse response
subplot(3,1,1);
stem(n,y);
xlabel('Time index n'); ylabel('Amplitude');
title('y[n]-0.75y[n-1]-0.25y[n-2]=x[n]'); grid;
hold on
% Compute the Step response y
s = filter(b,a,u);
% Plot the Step response
stem(n,s);
legend('Impulse Response','Step Response');
hold off
 
%% EX2,d
figure
% Compute the impulse response y
b = [1];
a = [1 -0.75 -0.5];
y = impz(b,a,N);
% Plot the impulse response
subplot(3,1,1);
stem(n,y);
xlabel('Time index n'); ylabel('Amplitude');
title('y[n]-0.75y[n-1]-0.5y[n-2]=x[n]'); grid;
hold on
% Compute the Step response y
s = filter(b,a,u);
% Plot the Step response
stem(n,s);
legend('Impulse Response','Step Response');
hold off

%% EX2,e

figure
% Compute the impulse response y
b = [1 -1 -1 +1];
a = [1];
y = impz(b,a,N);
% Plot the impulse response
subplot(3,1,1);
stem(n,y);
xlabel('Time index n'); ylabel('Amplitude');
title('y[n]=x[n]-x[n-1]-x[n-2]+x[n-3]'); grid;
hold on
% Compute the Step response y
s = filter(b,a,u);
% Plot the Step response
stem(n,s);
legend('Impulse Response','Step Response');
hold off
