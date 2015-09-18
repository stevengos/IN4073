/**
@author Steven Gosseling
*/

#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "joystick.h"
#include "../interface/packet.h"
#include "../interface/hamming.h"

/* current axis and button readings
 */
int	axis[6];
int	button[12];

/* Round division with int
 */
unsigned int round_div(unsigned int dividend, unsigned int divisor)
{
    return (dividend + (divisor / 2)) / divisor;
}

/* time
 */
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

int set_js_command(int fd, packet_t* p){
	struct js_event js;
	unsigned int	t, i, throttle_total = 0, throttle_on_scale = 0, command_set = 0;

	/* simulate work
	*/
	mon_delay_ms(20);
	t = mon_time_ms();

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
		return 1;


	printf(" |  ");

	//roll
	/*if(axis[0] < -JS_MIN_VALUE){
		printf("left  ");
	}
	else if(axis[0] > JS_MIN_VALUE){
		printf("right ");
	}
	else{
		printf("      ");
	}

	//pitch
	if(axis[1] < -JS_MIN_VALUE){
		printf("up   ");
	}
	else if(axis[1] > JS_MIN_VALUE){
		printf("down ");
	}
	else{
		printf("     ");
	}

	//yaw
	if(axis[2] < -JS_MIN_VALUE){
		printf("y_left  ");
	}
	else if(axis[2] > JS_MIN_VALUE){
		printf("y_right ");
	}
	else{
		printf("      ");
	}*/

	//throttle
	throttle_total = 65534 - (axis[3] + 32767);
	
	//printf("lift: %d ", throttle_total);

	if(throttle_total > JS_MIN_VALUE){
		//throttle_division = axis[3]/JS_STEP_DIVISION;
		throttle_on_scale = round_div(throttle_total,JS_STEP_DIVISION);

		if(throttle_on_scale > 1000){
			throttle_on_scale = 1000;
		}
		printf("lift: %d ", throttle_on_scale);

		p->header  = SET_LIFT;
		p->command = throttle_on_scale;

		command_set = 1;
	}

	if(command_set == 0){
		p->header  = EMPTY;
		p->command = EMPTY;
	}

	compute_hamming(p);

	return 0;
}
