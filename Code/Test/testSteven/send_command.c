#include "static.h"

int send_command(int board,char label, char command){
	int done = 0;

	packet p;
	p.label = label;
	p.command = command;

	do
	{
	done = write(board, &p, sizeof(p));
	}
	while(!done);

	return done;
}
