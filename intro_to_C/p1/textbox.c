/**
    @file textbox.c
    @author Maggie Lin (mclin)
    Prints given text with a border made of astericks 
    around it.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** Width of each line of text in the box. */
#define LINE_WIDTH 60

/** Symbol used to draw the border around the box. */
#define BORDER '*'

/**
    Makes a line with the exact line width so that the astericks 
    form a box shape.

    @return true if there is another input line to process.
  */
bool paddedLine()
{
    //Declare variables
    int nextChar = getchar();
    int charCount = 0;
    if (nextChar < 0) {
        return false;
    }
    //Put the left *
    putchar(BORDER);
    //putchar all characters that would fit
    while(nextChar != '\n' && nextChar > 0) {
        charCount = charCount + 1;
        if (charCount <= LINE_WIDTH) {
            putchar(nextChar);
        }
        nextChar = getchar();
    }
    //Check to see if spaces are needed
    if (charCount < LINE_WIDTH) {
        int spaces = LINE_WIDTH - charCount;
        for (int i = 0; i < spaces; i++) {
            putchar(' ');
        }
    }
    //Put the right *
    putchar(BORDER);
    printf("\n");
    //Return if there is a next line
    if (nextChar == '\n') {
        return true;
    } else {
        return false;
    }
}

/**
    Prints out multiple copies of the given character followed
    by a new line.
    
    @param ch character to be copied.
    @param count number of time the character needs to be copied.
  */
void lineOfChars( char ch, int count )
{
    for ( int i = 0; i < count; i++ ) {
        putchar(ch);
    }
    printf("\n");
}

/**
    Program starting point. Prints a border made of astericks 
    around given text.

    @return program exits successfully
  */
int main()
{
    lineOfChars('*', 62);
    while(paddedLine()) {
        paddedLine();
    }
    lineOfChars('*', 62);
    return EXIT_SUCCESS;

}
