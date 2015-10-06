#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
   	unsigned int c = 0x8000;
    	unsigned int g = 0x8000;
	
	int n = atoi(argv[1]);

    	for(;;)
	{
        	if (g*g > n)
   	         	g ^= c;

        	c >>= 1;

		printf("sqrt(%d) = %d\n", n, g);

        	if (c == 0)
		{
			printf("result: %d\n", g);
			return g;
        	}

        	g |= c;
    	}

	return 0;
}
