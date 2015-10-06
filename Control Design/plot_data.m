%% YAW CONTROL DESIGN
close all, clear all;

%% path
MEAS_PATH = '../Code/MAIN/measurements/';
file = 'meas_2015_10_02_12_18_53.txt';

path = fullfile(MEAS_PATH, file);
data = load_measures(path);

time = 0:size(data.time,1)-1;

%% motors
figure, plot(time,data.ae1, 'black', time,data.ae2, 'blue', ...
     time,data.ae3, 'red',   time,data.ae4, 'green');

legend('ae1','ae2','ae3','ae4');

%% acc
figure, plot(time,data.sax, 'black', time,data.say, 'blue', time,data.saz, 'red');

legend('sax','say','saz');

%% gyro
figure, plot(time,data.sp, 'black', time,data.sq, 'blue', time,data.sr, 'red');
hold on,plot(time,data.fp, 'black', time,data.fq, 'blue', time,data.fr, 'red');
legend('sp','sq','sr');

%% motor+sr
figure;
subplot 211, plot(time,data.ae1, 'black', time,data.ae2, 'blue');
subplot 212, plot(time,data.sr, 'red');

legend('ae1','ae2','sr');
