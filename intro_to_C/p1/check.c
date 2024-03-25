/** 
    @file check.c
    @author John Q Public (jqpublic)
    This program performs a few basic style checks on C source code
    read from standard input.  It looks for hard tab characters and for
    different types of line termination.
*/

#include <stdio.h>
#include <stdlib.h>

/** Exit code if style problems are encountered. */
#define STYLE_ERROR 255

/** True if the input contains windows-style line termination.
    (this is a global variable, something to avoid) */
int windowsNewlines = 0;

/** True if the input contains unix-style line termination. */
int unixNewlines = 0;

void detectNewline( int, int );

/** 
    Program starting point, reads characters from standard input until end-of-file.
    @param argc number of command-line arguments, not used.
    @param argv list of command-line arguments, not used.
    @return program exit status
*/
int main( int argc, char *argv[] )
{
    // Previous characters read, with EOF indicating no previous character.
    int prev = EOF;

    // Set to true if the file contains a tab.
    int containsTab = 0;

    // Normally, standard input will be in text mode, so we may not see
    // differences in line termination.  This should fix that.
    freopen( NULL, "rb", stdin );

    // Read all the input characters.
    for ( int ch = getchar(); ch != EOF; ch = getchar() ) {
        if ( ch == '\t' )
            containsTab = 1;
        
        detectNewline( prev, ch );

        prev = ch;
    }

    // Is there a style problem?
    if ( containsTab || windowsNewlines ) {
        if ( containsTab )
            printf( "This file contains hard tab characters\n" );

        if ( windowsNewlines ) {
            if ( unixNewlines )
                printf( "This file contains mixed types of line termination\n" );
            else
                printf( "This file contains windows line termination\n" );
        }

        return STYLE_ERROR;
    }

    // Did the last line have line termination?
    if ( prev != EOF && prev != '\n' )
        printf( "The last line of this file doesn't have line termination\n" );

    return EXIT_SUCCESS;
}

/**
   Given a two-character sequence, determine whether it looks like
   windows- or unix-style line termination.  Updates windowsNewlines
   or unixNewlines to indicate line termination encountered.
   
   @param prev previous character read, or EOF if there is no previous character.
   @param current most recent character read.
*/
void detectNewline( int prev, int current )
{
    if ( current == '\n' ) {
        if ( prev == '\r' )
            windowsNewlines = 1;
        else
            unixNewlines = 1;
    }
}