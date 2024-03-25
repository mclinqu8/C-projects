/**
    @file style.c
    @author Maggie Lin (mclin)
    Makes a paragraph of 103 randomly generated words with randomly 
    generated letters.
*/
#include <stdio.h>
#include <stdlib.h>

/** The line character limit for a single line in the paragraph. */
#define LINE_LIMIT 72

/**
    Makes a word of the given length with randomly generated letters.

    @param x number of characters contained in the randomly generated word.
*/
void printWord( int x )
{
    for ( int i = 0; i < x; i++ ) {
            // Print a random lower-case letter.
            printf( "%c", 97 + rand() % 26 );
    }
}

/**
    Makes a line with randomly generated words that does not 
    go over the 72 character limit.

    @return the count of how many words are in a line.
*/
int printLine() 
{
    int count = 0, pos = 0, space = 0;
    int len = 1 + rand() % 10;
    // Print a line of words up to a limited length.
    while ( pos + len + space < LINE_LIMIT ) {
        if ( space > 0 ) {
            printf( " " );
        }
        printWord( len );
        pos += len + space;
        len = 1 + rand() % 10;
        space = 1;
        count += 1;
    }
    printf( "\n" );
    return count;
}

/**
    Makes a paragraph with lines of randomly generated words.

    @param n number of lines in the paragraph.
    @return paragraph with n number of lines.
*/
int printParagraph( int n )
{
    int total = 0;
    for ( int i = 0; i < n; i++ ) {
    total += printLine();
    }
    return total;
}

/**
    Program starting point. Prints a paragraph with ten lines 
    of randomly generated words.

    @return program exits successfully
*/
int main()
{
    int w = printParagraph(10);
    printf( "Words: %d\n", w );
    return EXIT_SUCCESS;
    }
