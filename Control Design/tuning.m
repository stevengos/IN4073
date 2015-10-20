## Author: gsavaia <gsavaia@gsavaia-linux>
## Created: 2015-10-20

function []=tuning(scale_pitch, sp)

%scale_pitch = 500;
scale_lift = 8200;

lift_force = 127;
pitch_sensed = sp;

motor_rest =sqrt(  ( scale_lift*lift_force ) / 4 );

fprintf(['scale=' num2str(scale_pitch) '; sensor=' num2str(pitch_sensed) '\n\n']);

for K=1:25;
	pitch_momentum = -K*pitch_sensed;

	motor = sqrt(  ( scale_lift*lift_force + 2*scale_pitch*pitch_momentum ) / 4 );

	fprintf(['K=' num2str(K) ';\t pitch=' num2str(motor) ';\t diff=' num2str(motor_rest-motor) ';\t ratio=' num2str((motor_rest-motor)/1023*100)  ' \n'] );
end

endfunction
