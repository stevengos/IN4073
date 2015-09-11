
all: 
	lcc-x32 BOARD/main.c BOARD/commands.c BOARD/isr.c BOARD/mode.c -o main.ce
	gcc PC/transmitter.c PC/board.c -pthread -o terminal
