
#include "static.h"

#define JS_DEV	"/dev/input/js0"

/* current axis and button readings
 */
int	axis[6];
int	button[12];

void init_joystick_thread(pthread_t js_thread){
	
	int iret;

	iret = pthread_create(&js_thread, NULL, run_joystick, NULL);

	if(iret){
		fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
		exit(EXIT_FAILURE);
	}

	pthread_join( js_thread, NULL);
}

/* time
 */
#include <time.h>
#include <assert.h>
unsigned int    mon_time_ms(void)
{
        unsigned int    ms;
        struct timeval  tv;
        struct timezone tz;

        gettimeofday(&tv, &tz);
        ms = 1000 * (tv.tv_sec % 65); // 65 sec wrap around
        ms = ms + tv.tv_usec / 1000;
        return ms;
}

void    mon_delay_ms(unsigned int ms)
{
        struct timespec req, rem;

        req.tv_sec = ms / 1000;
        req.tv_nsec = 1000000 * (ms % 1000);
        assert(nanosleep(&req,&rem) == 0);
}

void *run_joystick(){
	printf("Init joystick\n");

	int 		fd;
	struct js_event js;
	unsigned int	t, i;

	if ((fd = open(JS_DEV, O_RDONLY)) < 0) {
		perror("jstest");
		exit(1);
	}

	/* non-blocking mode
	 */
	fcntl(fd, F_SETFL, O_NONBLOCK);

	while (1) {
		/* simulate work
		 */
		mon_delay_ms(300);
		t = mon_time_ms();
		
		pthread_mutex_lock(&lock_x);

		/* check up on JS
		 */
		while (read(fd, &js, sizeof(struct js_event)) == 
		       			sizeof(struct js_event))  {

			/* register data
			 */
			// fprintf(stderr,".");
			switch(js.type & ~JS_EVENT_INIT) {
				case JS_EVENT_BUTTON:
					button[js.number] = js.value;
					break;
				case JS_EVENT_AXIS:
					axis[js.number] = js.value;
					break;
			}
		}
		if (errno != EAGAIN) {
			perror("\njs: error reading (EAGAIN)");
			exit (1);
		}

		/*for(i = 0; i < 12; i++){
			if(button[i] == 1){
				printf("entered button: %i\n", i);			
			}		
		}*/

		pthread_mutex_unlock(&lock_x);

		printf("\n");
		printf("%5d   ",t);
		for (i = 0; i < 6; i++) {
			printf("%6d ",axis[i]);
		}
		printf(" |  ");
		for (i = 0; i < 12; i++) {
			printf("%d ",button[i]);
		}
		if (button[0])
			break;
	}
	printf("\n<exit>\n");
}
