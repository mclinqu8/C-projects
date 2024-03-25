/**
    @file trig.c
    @author Maggie Lin (mclin)
    Read an angle and make a table of successively more accurate 
    approximations of the sine, cosine, and tangent of the angle until 
    it reaches a certain accuracy threshold.
*/
#include <stdio.h>
#include <stdlib.h>

/** Approximation of PI.  For now, we're using our own constant for
    this. There's also one provided by the gnu math library, once we
    learn about that. */
#define PI 3.14159265358979323846

/** The accuracy threshold for termination of the table of sine, cosine,
    and tangent approximations. */
#define ACCURACY 0.000001

/**
    Get the angle from the user for approximation.

    @return the angle the user inputted if input is valid or return 
    an error message of "Invalid input" and exit the program.
  */
double getAngle()
{
    //The angle user will input
    double input;
    //Exit program if input is either not a double or input is not within valid range
    if (scanf("%lf", &input)) {
        if (input < -PI || input > PI) {
            printf("Invalid input\n");
            exit(1);
        }
    } else {
        printf("Invalid input\n");
        exit(1);
    }
    return input;
}

/**
    Get the difference value from the two most recent sine and cosine.
    
    @param a first double to get the difference from.
    @param b second double to get the difference from.
    @return difference of the first and second input.
  */
double difference( double a, double b )
{
    double difference;
    difference = a - b;
    if (difference < 0) {
        difference = - difference;
    }
    return difference;
}

/**
    Prints the table header for the table of sine, cosine,
    and tangent approximations.
  */
void tableHeader()
{
    printf("terms |         sin |         cos |         tan \n");
    printf("------+-------------+-------------+-------------\n");
}

/**
    Get the constant value for the sine Taylor series approximation equation
    depending on the term value.
    
    @param term term at which the Taylor series is at.
    @return the constant value at the term for the sine Taylor series
    equation.
  */
int sinConst(int term) 
{
    //Constant in the Taylor Series for sine
    int constant;
    if ((term - 1) % 2 == 0) {
        constant = 0;
    } else {
        if (((term - 1) / 2) % 2 == 0) {
            constant = 1;
        } else {
            constant = -1;
        }
    }
    return constant;
}

/**
    Get the constant value for the cosine Taylor series approximation equation
    depending on the term value.
    
    @param term term at which the Taylor series is at.
    @return the constant value at the term for the cosine Taylor series
    equation.
  */
int cosConst(int term) 
{
    //Constant in the Taylor Series for cosine
    int constant;
    if ((term - 1) % 2 == 1) {
        constant = 0;
    } else {
        if (((term - 1) / 2) % 2 == 0) {
            constant = 1;
        } else {
            constant = -1;
        }
    }
    return constant;
}

/**
    Get the exponential value for the Taylor series approximation equation 
    depending on the term value.
    
    @param term term at which the Taylor series is at.
    @param input input from user which represents the angle to be approximated.
    @return the exponential value at the term for the the Taylor series 
    approximation equation.
  */
double exponential(int term, double input) 
{
    double total = 0;
    if ((term - 1) != 0) {
        total = 1;
    }
    for (int i = 1; i < term; i++) {
        total *= input;
    }
    return total;
}

/**
    Get the factorial value for the Taylor series approximation equation 
    depending on the term value.
    
    @param term term at which the Taylor series is at.
    @return the factorial value at the term for the the Taylor series 
    approximation equation.
  */
double factor(int term) 
{
    double factor = 1;
    for (int i = 1; i < term; i++) {
        factor *= i;
    }
    return factor;
}


/**
    Get the sine approximation of the angle using the 
    Taylor series approximation equation at the term value.
    
    @param term term at which the Taylor series is at.
    @param input input from user which represents the angle to be approximated.
    @return the approximated value of the sine of the angle based on the term at 
    which the Taylor series is at.
  */
double sinCalc(int term, double input) 
{
    double total = 0;
    for (int i = 1; i <= term; i++) {
        total += sinConst(i) * (exponential(i, input) / factor(i));
    }
    return total;
}

/**
    Get the cosine approximation of the angle using the 
    Taylor series approximation equation at the term value.
    
    @param term term at which the Taylor series is at.
    @param input input from user which represents the angle to be approximated.
    @return the approximated value of the cosine of the angle based on the term at 
    which the Taylor series is at.
  */
double cosCalc(int term, double input) 
{
    double total = 1;
    for (int i = 1; i <= term; i++) {
        total += cosConst(i) * (exponential(i, input) / factor(i));
    }
    return total;
}

/**
    Get the tangent approximation of the angle using the sine and cosine
    approximation.
    
    @param sine sine approximation of the angle.
    @param cos cosine approximation of the angle.
    @return the tangent approximation of the angle.
  */
double tanCalc(double sine, double cos) 
{
    double total = 0;
    total = sine / cos;
    return total;
}

/**
    Print the header for the table of successively more accurate 
    approximations of the sine, cosine, and tangent of the angle. 
  */
void tableRow( int terms, double s, double c, double t )
{
    printf("%5d | %11.7lf | %11.7lf | %11.7lf\n", terms, s, c, t);
}

/**
    Program starting point. Prints a table of successively more accurate 
    approximations of the sine, cosine, and tangent of the angle using the 
    Taylor Series equation.

    @return program exits successfully
  */
int main()
{
    int terms = 1;
    double prevSine = 0;
    double sine = 0;
    double prevCos = 0;
    double cos = 0;
    double tan = 0;
    double input = getAngle();
    tableHeader();
    // //First term
    prevSine = sinCalc(terms, input);
    prevCos = cosCalc(terms, input);
    tan = tanCalc(prevSine, prevCos);
    tableRow(terms, prevSine, prevCos, tan);
    // //Second term
    terms++;
    sine = sinCalc(terms, input);
    cos = cosCalc(terms, input);
    tan = tanCalc(sine, cos);
    tableRow(terms, sine, cos, tan);
    //Terms after the second term
    while ((difference(prevSine, sine) > ACCURACY) || (difference(prevCos, cos) > ACCURACY)) {
        terms++;
        prevSine = sine;
        sine = sinCalc(terms, input);
        prevCos = cos;
        cos = cosCalc(terms, input);
        tan = tanCalc(sine, cos);
        tableRow(terms, sine, cos, tan);
    }
    return EXIT_SUCCESS;
}
