CC = gcc
CFLAGS = -Wall -std=c99 -g

#This is the default target
all: polish10 polish32

#Building for each base case
polish10: polish.o base10.o bounds.o
	gcc polish.o base10.o bounds.o -o polish10

polish32: polish.o base32.o bounds.o
	gcc polish.o base32.o bounds.o -o polish32

#Building each object file
polish.o: polish.c base.h bounds.h
base10.o: base10.c base.h base.h bounds.h
base32.o: base32.c base.h base.h bounds.h
bounds.o: bounds.c bounds.h

clean:
	rm -f base10.o
	rm -f base32.o
	rm -f polish.o
	rm -f bounds.o
	rm -f output.txt
	rm -f polish10
	rm -f polish32 

