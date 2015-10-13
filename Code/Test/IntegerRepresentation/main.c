#include <stdio.h>

int main()
{
	short value = 128;
	short result = 0;

	unsigned char cl, cr;
	unsigned short mask = 0x00ff;

for(value=0; value<512; value++){
	cr = value;
	cl = (value >> 8);

	printf("sent %x = (%d,%d) => ", value, cl, cr);
	
	result = 0;
	result = result | cl;
	result = result << 8;
	result = result | cr;

	printf("%d\n", result);
}
	return 0;
}
