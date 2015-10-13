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

/* current joystick modes
 */

int js_init_mode = 1;
int js_init_2_op_counter = 0;
int js_operation_mode = 0;

/* Round division with int */
inline int round_div(int dividend, int divisor)
{
    return (dividend + (divisor / 2)) / divisor;
}

/* time */
unsigned int mon_time_ms(void)
{
    unsigned int    ms;
    struct timeval  tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);
    ms = 1000 * (tv.tv_sec % 65); // 65 sec wrap around
    ms = ms + tv.tv_usec / 1000;
    return ms;
}

void mon_delay_ms(unsigned int ms)
{
    struct timespec req, rem;

    req.tv_sec = ms / 1000;
    req.tv_nsec = 1000000 * (ms % 1000);
    assert(nanosleep(&req,&rem) == 0);
}

void set_throttle_command(char header, int throttle, int divisor)
{
	float throttle_on_scale = 0, multiplier;
	char send_value;

	throttle_on_scale = round_div(throttle,divisor);

	if(throttle_on_scale > 1000)
        throttle_on_scale = 1000;

	else if(throttle_on_scale < -1000)
		throttle_on_scale = -1000;

	//scale int to char
	if(divisor == JS_STEP_DIVISION_SMALL)
		multiplier = 127;
	else
		multiplier = 255;

	send_value = throttle_on_scale/1000.0*multiplier;

    if(header == SET_LIFT)
        printf("Lift>>\t Joystick=%f\t Command=%d\t MAX=%f \n", throttle_on_scale, (unsigned char)send_value, multiplier);
    if(header == SET_PITCH)
        printf("Pitch>>\t Joystick=%f\t Command=%d\t MAX=%f \n", throttle_on_scale, send_value, multiplier);
    if(header == SET_ROLL)
        printf("Roll>>\t Joystick=%f\t Command=%d\t MAX=%f \n", throttle_on_scale, send_value, multiplier);
    if(header == SET_YAWRATE)
        printf("Yaw>>\t Joystick=%f\t Command=%d\t MAX=%f \n", throttle_on_scale, send_value, multiplier);

	//send the packet
	//printf(" %c = %d | ", header, send_value);

	push_packet_t(header, send_value);
}

int set_js_command(int fd)
{
	struct js_event js;
	unsigned int t, i, throttle_total = 0, throttle_on_scale = 0, command_set = 0;

	/* simulate work ???*/
	mon_delay_ms(50);
	t = mon_time_ms();

	/*************** READ JOYSTICK ****************************/

	while (read(fd, &js, sizeof(struct js_event)) == sizeof(struct js_event))
	{
		/* register data */
		switch(js.type & ~JS_EVENT_INIT)
		{
			case JS_EVENT_BUTTON:
                        button[js.number] = js.value;
                        break;

			case JS_EVENT_AXIS:
                        axis[js.number] = js.value;
                        break;
		}
	}

	if (errno != EAGAIN)
	{
		perror("\njs: error reading (EAGAIN)");
		exit(1);
	}

	//exit program *********** SHOULD GO IN PANIC MODE?
	if (button[0])
		return 1;

    /*************** ENABLE JOYSTICK AFTER EVERYTHING IS SET TO ZERO *****************/

	//when all axis are 0, then it is safe to increase the init_2_op counter
	if(js_init_mode && axis[0] == 0 && axis[1] == 0 && axis[2] == 0)
		js_init_2_op_counter++;

	// when a level of init_2_op of 5 is reached the joystick is considered out of biased init_mode
	if(js_init_2_op_counter > JS_OPERATION_MODE_THRESHOLD)
		js_init_mode = 0, js_operation_mode = 1;

	if(js_init_mode)
		printf("init_mode\n");

	if(js_operation_mode)
		printf("op mode\n");

    /*************** PREPARE PACKET TO BE SENT *************************************/

	//pitch
	if(axis[1] < -JS_MIN_VALUE || axis[1] > JS_MIN_VALUE)
	{
		set_throttle_command(SET_PITCH, axis[1], JS_STEP_DIVISION_SMALL);
		command_set = 1;
	}
	else
	{
		set_throttle_command(SET_PITCH, 0, JS_STEP_DIVISION_SMALL);
		command_set = 1;
	}

    //roll
	if(axis[0] < -JS_MIN_VALUE || axis[0] > JS_MIN_VALUE)
	{
		set_throttle_command(SET_ROLL, axis[0], JS_STEP_DIVISION_SMALL);
		command_set = 1;
	}
	else
	{
		set_throttle_command(SET_ROLL, 0, JS_STEP_DIVISION_SMALL);
		command_set = 1;
	}

	//yaw
	if(axis[2] < -JS_MIN_VALUE || axis[2] > JS_MIN_VALUE){
		set_throttle_command(SET_YAWRATE, axis[2], JS_STEP_DIVISION_SMALL);

		command_set = 1;
	}else{
		set_throttle_command(SET_YAWRATE, 0, JS_STEP_DIVISION_SMALL);

		command_set = 1;
	}

	//throttle
	throttle_total = 65534 - (axis[3] + 32767);

	if(throttle_total > JS_MIN_VALUE){
		set_throttle_command(SET_LIFT, throttle_total, JS_STEP_DIVISION_BIG);

		command_set = 1;		
	}else{
		set_throttle_command(SET_LIFT, 0, JS_STEP_DIVISION_BIG);

		command_set = 1;
	}

	if(command_set == 0){
		push_packet_t(EMPTY, EMPTY);
	}	


	return 0;
}
