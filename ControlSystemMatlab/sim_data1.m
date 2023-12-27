close all;
clear all;

% 1. Передаточная функция
num = [1 4 4 2];
den = [1 4 4 1];
W = tf(num, den);

% 2. Модель вход-состояние-выход в непрерывном виде
[Ac,Bc,Cc,Dc] = tf2ss(num, den);
S = ss(W);

% 3. Частоты дискретизации
fs1 = 5;
fs2 = 30;
fs3 = 100;

% 4. Периоды дискретизации
dt1 = 1/fs1;
dt2 = 1/fs2;
dt3 = 1/fs3;

% 5. Модель вход-состояние-выход в дискретном виде
% 5.1 Для t1
Ad1 = expm(Ac*dt1);
Bd1 = inv(Ac)*(Ad1 - eye(3,3))*Bc;
Cd1 = Cc;
Dd1 = Dc;

% 5.2 Для t2
Ad2 = expm(Ac*dt2);
Bd2 = inv(Ac)*(Ad2 - eye(3,3))*Bc;
Cd2 = Cc;
Dd2 = Dc;

% 5.3 Для t3
Ad3 = expm(Ac*dt3);
Bd3 = inv(Ac)*(Ad3 - eye(3,3))*Bc;
Cd3 = Cc;
Dd3 = Dc;



