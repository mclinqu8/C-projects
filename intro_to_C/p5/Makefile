CC = gcc
CFLAGS = -Wall -std=c99 -g

#Building main


hash: hash.o byteBuffer.o ripeMD.o
	gcc hash.o byteBuffer.o ripeMD.o -o hash

testdriver: 
	gcc -Wall -std=c99 -g -DTESTABLE testdriver.c ripeMD.c byteBuffer.c -o testdriver 

#Building each object file
hash.o: hash.c ripeMD.h byteBuffer.h
ripeMD.o: ripeMD.c ripeMD.h byteBuffer.h
byteBuffer.o: byteBuffer.c byteBuffer.h

clean:
	rm -f output.txt
	rm -f stderr.txt
	rm -f stdout.txt
	rm -f hash.o
	rm -f ripeMD.o
	rm -f byteBuffer.o
	rm -f testdriver
	rm -f hash
	rm -f ripeMD
	rm -f byteBuffer
