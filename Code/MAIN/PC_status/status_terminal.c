#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int main(){
	while(1){
		printf("test\n");
		usleep(50000); //sleep 50ms
		system("clear");
	}
	return 0;
}
