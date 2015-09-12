
all: 
	lcc-x32 BOARD/main.c BOARD/drone.c BOARD/isr.c BOARD/command.c -o main.ce
	gcc PC/transmitter.c PC/board.c -pthread -o terminal
