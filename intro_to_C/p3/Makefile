CC = gcc
CFLAGS = -Wall -std=c99 -g

#This is the default target
all: replace

#Building main
replace: replace.o expand.o line.o
	gcc replace.o expand.o line.o -o replace

#Building each object file
replace.o: replace.c expand.h line.h
expand.o: expand.c expand.h
line.o: line.c line.h

clean:
	rm -f expand.o
	rm -f line.o
	rm -f replace
	rm -f replace.o
	rm -f line
	rm -f output.txt
	rm -f stderr.txt
	rm -f stdout.txt
