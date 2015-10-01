#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int motor_2 = atof(argv[1]);
	int motor = motor_2/650; //sqrt
	motor = !motor ? 1 : motor;
	int prev = -100;
	int i=0;

	while( prev - motor > 2 || prev - motor < -2 )
		prev = motor, motor = (motor + motor_2/motor) >> 1, printf("sqrt(%d) = %d, actual = %f\n", motor_2, motor, sqrt(motor_2) );

	return 0;
}

