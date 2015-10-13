%% YAW CONTROL DESIGN
close all, clear all;

%% path
MEAS_PATH = '../Code/MAIN/measurements/';
file = 'meas_2015_10_09_11_44_04.txt';

path = fullfile(MEAS_PATH, file);
data = load_filters(path);

time = 0:size(data.time,1)-1;

%% motors
% figure, plot(time,data.ae1, 'black', time,data.ae2, 'blue', ...
%      time,data.ae3, 'red',   time,data.ae4, 'green');
% 
% legend('ae1','ae2','ae3','ae4');

%% acc
figure, plot(time,data.sax, 'red'); hold on; plot(time,data.fax, 'blue');
figure, plot(time,data.say, 'red'); hold on; plot(time,data.fay, 'blue');
figure, plot(time,data.saz, 'red'); hold on; plot(time,data.faz, 'blue');
legend('sax','say','saz');

%% gyro
figure, plot(time,data.sp, 'red'); hold on; plot(time,data.fp, 'blue');
figure, plot(time,data.sq, 'red'); hold on; plot(time,data.fq, 'blue');
figure, plot(time,data.sr, 'red'); hold on; plot(time,data.fr, 'blue');
legend('sp','sq','sr');

