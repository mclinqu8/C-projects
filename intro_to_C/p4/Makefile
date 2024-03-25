CC = gcc
CFLAGS = -Wall -std=c99 -g

#This is the default target
all: kiosk

#Building main
kiosk: kiosk.o menu.o input.o
	gcc kiosk.o menu.o input.o -o kiosk

#Building each object file
kiosk.o: kiosk.c input.h menu.h
menu.o: menu.c menu.h input.h
input.o: input.c input.h

clean:
	rm -f output.txt
	rm -f stderr.txt
	rm -f stdout.txt
	rm -f menu.o
	rm -f input.o
	rm -f kiosk.o
	rm -f kiosk
