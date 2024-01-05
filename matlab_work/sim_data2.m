clear all
close all

% 1. Исходная функция
syms t;
y = cos((-1/2)*t);

z1 = diff(y,1);
z2 = diff(y,2);

% 2. Модель вход-состояние-выход
A = [ 0    1; 
     -0.25 0];
B = [0;
     0];
C = [1 0];
D = [0];

% 3. Частоты дискретизации
fs1 = 5;
fs2 = 30;
fs3 = 100;

% 4. Периоды дискретизации
dt1 = 1/fs1;
dt2 = 1/fs2;
dt3 = 1/fs3;

% 5. Дискретная модель
system_contin = ss(A, B, C, D);
system_disc1 = c2d(system_contin, dt1);
system_disc2 = c2d(system_contin, dt2);
system_disc3 = c2d(system_contin, dt3);
