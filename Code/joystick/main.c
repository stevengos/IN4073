#include "static.h"

void main(){
	char c;
	pthread_t js_thread;

	init_joystick_thread(js_thread);

	while(1){
		scanf("%c ", &c);
		
		if(c != -1){
			pthread_mutex_lock(&lock_x);

			printf("You entered %c\n", c);

			pthread_mutex_unlock(&lock_x);
		}
	}

	exit(EXIT_SUCCESS);
}
