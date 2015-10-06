#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
	int n = atoi(argv[1]);
	int x = n;

	int xhalf = x >> 1;
	int i = x; // get bits for floating value
	i = 0x5f3759df - (i>>1); // gives initial guess y0
	x = i; // convert bits back to float
	x = x*(1-xhalf*x*x); // Newton step, repeating increases accuracy
	
	printf("sqrt(%d) = %d\n", n, n*x);
	
	return n*x;
}
