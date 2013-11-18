CC=msp430-gcc
CFLAGS=-Os -Wall -g -O0 -mmcu=msp430g2553
#CFLAGS=-Os -Wall -g -O0 -mmcu=msp430g2452 
OBJS=$(v).o

main: main.c
	$(CC) $(CFLAGS) -o main main.c

clean:
	rm main

test: main
	mspdebug rf2500 "prog main"
