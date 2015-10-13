
all: 
	lcc-x32 BOARD/*.c interface/*.c -o main.ce
	gcc PC/*.c interface/*.c -pthread -o terminal
	gcc PC_status/*.c interface/*.c -pthread -o status_terminal
