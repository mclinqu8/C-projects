CC = gcc
CFLAGS = -Wall -std=c99 -g

#This is the default target
all: interpret

#Building main
interpret: interpret.o parse.o syntax.o value.o
	gcc interpret.o parse.o syntax.o value.o -o interpret

#Building each object file
interpret.o: interpret.c parse.h syntax.h value.h
parse.o: parse.c parse.h syntax.h value.h
syntax.o: syntax.c syntax.h value.h
value.o: value.c value.h

clean:
	rm -f output.txt
	rm -f stderr.txt
	rm -f stdout.txt
	rm -f interpret.o
	rm -f parse.o
	rm -f syntax.o
	rm -f value.o 
	rm -f interpret
