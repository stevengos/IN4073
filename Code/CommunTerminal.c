/*------------------------------------------------------------
 * Simple terminal in C
 *
 * Arjan J.C. van Gemund (+ few mods by Mark Dufour)
 *------------------------------------------------------------
 */
#define	FALSE		0
#define	TRUE		1

#include <stdio.h>
#include <termios.h>
#include <unistd.h>


/*------------------------------------------------------------
 * console I/O
 *------------------------------------------------------------
 */
struct termios 	savetty;

void	term_initio()
{
	struct termios tty;

        tcgetattr(0, &savetty);
        tcgetattr(0, &tty);
        tty.c_lflag &= ~(ECHO|ECHONL|ICANON|IEXTEN);
        tty.c_cc[VTIME] = 0;
        tty.c_cc[VMIN] = 0;
        tcsetattr(0, TCSADRAIN, &tty);
}

void	term_exitio()
{
	tcsetattr(0, TCSADRAIN, &savetty);
}

void	term_puts(char *s)
{
	fprintf(stderr,"%s",s);
}

void	term_putchar(char c)
{
	putc(c,stderr);
}

int	term_getchar_nb()
{
        static unsigned char 	line [2];

	// note: destructive read

        if (read(0,line,1)) {
                return (int) line[0];
        }
        return -1;
}

int	term_getchar()
{
        int    c;

        while ((c = term_getchar_nb()) == -1)
                ;
        return c;
}

/*------------------------------------------------------------
 * serial I/O (8 bits, 1 stopbit, no parity, 115,200 baud)
 *------------------------------------------------------------
 */
#include <termios.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#define SERIAL_DEVICE 	"/dev/ttyS0"


int fd_RS232;


int rs232_open(void)
{
  	char 		*name;
  	int 		result;
  	struct termios	tty;

        fd_RS232 = open(SERIAL_DEVICE, O_RDWR | O_NOCTTY);
        assert(fd_RS232>=0);

  	result = isatty(fd_RS232);
  	assert(result == 1);

  	name = ttyname(fd_RS232);
  	assert(name != 0);

  	result = tcgetattr(fd_RS232, &tty);
  	assert(result == 0);

        tty.c_iflag = IGNBRK; /* ignore break condition */
        tty.c_oflag = 0;
        tty.c_lflag = 0;

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; /* 8 bits-per-character */
        tty.c_cflag |= CLOCAL | CREAD; /* Ignore model status + read input */

        cfsetospeed(&tty, B115200); /* set output baud rate */
        cfsetispeed(&tty, B115200); /* set input baud rate */

        tty.c_cc[VMIN]  = 0;
        tty.c_cc[VTIME] = 0;

        tty.c_iflag &= ~(IXON|IXOFF|IXANY);

        result = tcsetattr (fd_RS232, TCSANOW, &tty); /* non-canonical */

        tcflush(fd_RS232, TCIOFLUSH); /* flush I/O buffer */
}


int 	rs232_close(void)
{
  	int 	result;

  	result = close(fd_RS232);
  	assert (result==0);
}


int	rs232_getchar_nb()
{
	int 		result;
	unsigned char 	c;

	result = read(fd_RS232, &c, 1);
	if (result == 0) {
		return -1;
	}
	else {
		assert(result == 1);
		return (int) c;
	}
}


int 	rs232_getchar()
{
	int 	c;

	while ((c = rs232_getchar_nb()) == -1);

	return c;
}


int 	rs232_putchar(char c)
{
	int result;

	do {
		result = (int) write(fd_RS232, &c, 1);
	} while (result == 0);

	assert(result == 1);
	return result;
}




/*----------------------------------------------------------------
 * main -- execute terminal
 *----------------------------------------------------------------
 */
int main(void)
{
	char	c;

	term_initio();
	rs232_open();

	term_puts("Terminal program (Embedded Software Lab)\n");
	term_puts("Type ^C to exit\n");

	/* discard any incoming text
	 */
	while ((c = rs232_getchar_nb()) != -1)
		fputc(c,stderr);

	for (;;) {
		if ((c = term_getchar_nb()) != -1) {
			rs232_putchar(c);
		}
		if ((c = rs232_getchar_nb()) != -1) {
			term_putchar(c);
		}
	}
	term_exitio();
	rs232_close();
	term_puts("\n<exit>\n");
  	exit(0);
}


