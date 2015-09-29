#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int motor_2 = atof(argv[1]);
	int motor = motor_2/2; //sqrt

	int i = 0;

	for(i=0; i<15; i++)
		motor = (motor + motor_2/motor)/2, printf("sqrt(%d) = %d, actual = %f\n", motor_2, motor, sqrt(motor_2) );

	return 0;
}

