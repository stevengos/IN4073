function [data] = plot_data(file)

%% path
MEAS_PATH = '../Code/MAIN/measurements/';
%file = 'meas_2015_10_23_10_45_27.txt';

path = fullfile(MEAS_PATH, file);
data = load_measures(path);

time = 0:size(data.time,1)-1;

%% motors
% figure, plot(time,data.ae1, 'color', 'black', time,data.ae2, 'color', 'blue', ...
%              time,data.ae3, 'color', 'red',   time,data.ae4, 'color', 'green');
% legend('ae1','ae2','ae3','ae4');

%% acc
figure, plot(time,data.sax, 'color', 'red');hold on; plot(time,data.fax, 'color', 'blue'); title('ax');
figure, plot(time,data.say, 'color', 'red'); hold on; plot(time,data.fay, 'color', 'blue'); title('ay');
figure, plot(time,data.saz, 'color', 'red'); hold on; plot(time,data.faz, 'color', 'blue'); title('az');


%% gyro
figure, plot(time,data.sp, 'color', 'red'); hold on; plot(time,data.fp, 'color', 'blue'); title('sp');
figure, plot(time,data.sq, 'color', 'red'); hold on; plot(time,data.fq, 'color', 'blue'); title('sq');
figure, plot(time,data.sr, 'color', 'red'); hold on; plot(time,data.fr, 'color', 'blue'); title('sr');


endfunction
