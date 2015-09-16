
all: 
	lcc-x32 BOARD/main.c BOARD/drone.c BOARD/isr.c BOARD/command.c interface/hamming.c -o main.ce
	gcc PC/transmitter.c PC/board.c interface/hamming.c -pthread -o terminal
