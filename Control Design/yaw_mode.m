%% YAW MODE
clear all, close all
t = 0:100; t=t';

coeff = [+2 +0 -1 -1; ...
         +0 -2 +1 -1; ...
         -2 +0 -1 -1; ...
         +0 +2 +1 -1];

% SCALE PARAMETERS
scale_lift = -16400;
scale_pitch = 8240;
scale_roll = 8240;
scale_yaw = 16400;
scales = diag( [scale_pitch, scale_roll, scale_yaw, scale_lift] );

% CONTROLLER PARAMETERS
Kp = 1;
Kr = 1;
Ky = 3;

% SENSORS
MEAS_PATH = '../Code/MAIN/measurements/';
file = 'meas_2015_10_09_10_12_12.txt';
path = fullfile(MEAS_PATH, file);
data = load_measures(path);

ax = data.sax;
ay = data.say;
az = data.saz;
p = data.sp;
q = data.sq;
r = data.sr;

N = size(data.sr,1);

momentums = [zeros(1,N);...
            zeros(1,N);...
            zeros(1,N);...
            500*ones(1,N)];

references = zeros(4, N);

for i=1:size(data.sr,1)
    e(i) = references(3, i) - r(i);
    momentums(3,i) = 3 * e(i);
    
    motors(:,i) = 1/4.*coeff*scales*momentums(:,i);
    motors( motors < 0 ) = 0;
    motors = sqrt(motors);
end

figure, plot(e);
figure, plot(motors);
    
