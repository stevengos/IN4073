%% YAW CONTROL DESIGN
close all, clear all;

%% path
MEAS_PATH = '../Code/MAIN/measurements/';
file = 'meas_2015_10_09_11_44_04.txt';

path = fullfile(MEAS_PATH, file);
data = load_filters(path);

t = 0:size(data.time,1)-1;

figure, plot(t,data.fax,'blue',t,data.sax,'red');

%%filter MA 
N = 5;
A = 1;
B = ones(N,1)./N;
filtered = filter(B,A,data.sax);
figure, plot(t,filtered,'blue',t,data.sax,'red');

%%filter BUTTER
N = 100;
A = [1 -0.9];
B = 1-0.9;
filtered = filter(B,A,data.sax);
figure, plot(t,filtered,'blue',t,data.sax,'red');
F = tf(B,A,1)
figure, bode(F);
figure, lsim(F,data.sax);

%%FIXED POINT
S = 1; W = 16; D = 8;
fp_ax = fi(data.sax, S, W, D);
H = fdesign.lowpass('N,F3db',1,0.1052);
Hd = design(H,'butter');
filtered = filter(Hd,fp_ax);
figure, plot(t,filtered,'blue',t,data.sax,'red');