#ifndef __HARKLEMATH__
#define __HARKLEMATH__

#include <fenv.h>
#include "Harklecurse.h"            // hcCartCoord_ptr
#include <stdbool.h>                // bool, true, false

typedef struct cartesianCoordinate
{
    double xCoord;  // X coordinate
    double yCoord;  // Y coordinate
} cartPnt, *cartPnt_ptr;

typedef struct hmLineLengthCalculation
{
    int xCoord;   // Absolute x coordinate
    int yCoord;   // Absolute y coordinate
    double dist;  // Distance from the current point
} hmLineLen, *hmLineLen_ptr;

// double is a 64 bit IEEE 754 double precision Floating Point Number (1 bit for the sign, 11 bits for the exponent, and 52* bits for the value), i.e. double has 15 decimal digits of precision.
#define DBL_PRECISION 15

// Rounding MACROs to pass as round_a_dble()'s rndDir argument
#define HM_RND FE_TONEAREST         // Round to nearest (the default)
#define HM_UP FE_UPWARD             // Round up (toward positive infinity)
#define HM_DWN FE_DOWNWARD          // Round down (toward negative infinity)
#define HM_IN FE_TOWARDZERO         // Round toward zero

// Center MACROs to pass as determine_center()'s orientWint argument
#define HM_UP_LEFT   1              // If not exactly center, window's center defaults to upper left
#define HM_UP_RIGHT  2              // If not exactly center, window's center defaults to upper right
#define HM_LOW_LEFT  3              // If not exactly center, window's center defaults to lower left
#define HM_LOW_RIGHT 4              // If not exactly center, window's center defaults to lower left

// Approximate (guesstimated) number of primes less than data type maximums
#define NUM_PRIMES_ULLONG 455050683  // Just a wag based on http://primes.utm.edu/howmany.html#pnt


//////////////////////////////////////////////////////////////////////////////
/////////////////////// FLOATING POINT FUNCTIONS START ///////////////////////
//////////////////////////////////////////////////////////////////////////////


/*
    PURPOSE - Abstract the process of rounding a double to an int
    INPUT
        roundMe - The double that needs to be rounded
        rndDir - The direction to round the double:
            Use the HM_* MACROS above or
            Use the original MACROS from fenv.h or
            Pass 0 to utilize the default settings
    OUTPUT
        On success, the integer representation of the rounded double
        On failure, 0
    NOTES
        If rndDir is invalid, round_a_dble() will default to the current env
 */
int round_a_dble(double roundMe, int rndDir);


/*
    PURPOSE - Determine if one double is greater than another to a certain
        level of precision
    INPUT
        x - Is x greater than...
        y - ...y...
        precision - ...considering this many decimal places?
    OUTPUT
        If x > y, true
        Otherwise, false
 */
bool dble_greater_than(double x, double y, int precision);


/*
    PURPOSE - Determine if one double is less than another to a certain
        level of precision
    INPUT
        x - Is x less than...
        y - ...y...
        precision - ...considering this many decimal places?
    OUTPUT
        If x < y, true
        Otherwise, false
 */
bool dble_less_than(double x, double y, int precision);


/*
    PURPOSE - Determine if one double is equal to another to a certain
        level of precision
    INPUT
        x - Is x equal...
        y - ...y...
        precision - ...considering this many decimal places?
    OUTPUT
        If x == y, true
        Otherwise, false
 */
bool dble_equal_to(double x, double y, int precision);


/*
    PURPOSE - Determine if one double is NOT equal to another to a certain
        level of precision
    INPUT
        x - Is x NOT equal...
        y - ...y...
        precision - ...considering this many decimal places?
    OUTPUT
        If x != y, true
        Otherwise, false
    NOTES
        This function calls and returns the opposite of:
            dble_equal_to()
 */
bool dble_not_equal(double x, double y, int precision);


/*
    PURPOSE - Determine if one double is greater than or equal to
        another to a certain level of precision
    INPUT
        x - Is x greater than or equal to...
        y - ...y...
        precision - ...considering this many decimal places?
    OUTPUT
        If x >= y, true
        Otherwise, false
    NOTES
        This function will call the following functions in this order:
            dble_equal_to()
            dble_greater_than()
 */
bool dble_greater_than_equal_to(double x, double y, int precision);


/*
    PURPOSE - Determine if one double is less than another to a certain
        level of precision
    INPUT
        x - Is x less than or equal to...
        y - ...y...
        precision - ...considering this many decimal places?
    OUTPUT
        If x <= y, true
        Otherwise, false
    NOTES
        This function will call the following functions in this order:
            dble_equal_to()
            dble_less_than()
 */
bool dble_less_than_equal_to(double x, double y, int precision);


/*
    PURPOSE - Extricate precision 'mask' calculation from the floating point
        functions
    INPUT
        precision - Number of decimal points of precision desired
    OUTPUT
        On success, a double value to be used as a comparative value
        On failure, 0
    NOTES
        This function will attempt to determine/calculate the maximum level
            of precision for the system it is compiled on.
        I haven't yet determined how I want to handle TOO much desired
            precision.
        Also, I haven't yet determined how I want to handle the system's
            maximum level of precision if the caller's desired level of
            precision exceeds the system's maximum precision.
 */
double calc_precision(int precision);



//////////////////////////////////////////////////////////////////////////////
/////////////////////// FLOATING POINT FUNCTIONS STOP ////////////////////////
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
////////////////////////// GEOMETRIC FUNCTIONS START /////////////////////////
//////////////////////////////////////////////////////////////////////////////


/*
    PURPOSE - Calculate a point on an elipse given a, b, and y for the
        standard equation of an elipse:
            x²   y²
            ─  + ─  = 1
            a²   b²
    INPUT
        aVal - "a" from the standard equation above
        bVal - "b" from the standard equation above
        yVal - "y" from the standard equation above
    OUTPUT
        On success, "x" from the equation above
    NOTES
        This function only returns the positive result.  Return value
            could also be negative.
 */
double calc_ellipse_x_coord(double aVal, double bVal, double yVal);


/*
    PURPOSE - Calculate a point on an elipse given a, b, and y for the
        standard equation of an elipse:
            x²   y²
            ─  + ─  = 1
            a²   b²
    INPUT
        aVal - "a" from the standard equation above
        bVal - "b" from the standard equation above
        xVal - "x" from the standard equation above
    OUTPUT
        On success, "y" from the equation above
    NOTES
        This function only returns the positive result.  Return value
            could also be negative.
 */
double calc_ellipse_y_coord(double aVal, double bVal, double xVal);


/*
    PURPOSE - Calculate a set of points on an ellipse given the form:
            x²   y²
            ─  + ─  = 1
            a²   b²
    INPUT
        aVal - "a" from the standard equation above
        bVal - "b" from the standard equation above
        numPnts - Pointer to an int variable in which to store the number
            of individual points, not to be confused w/ coordinates, in
            the returned array
    OUTPUT
        On success, a heap-allocated array
        On failure, 0 (since x can never be zero for a centered ellipse)
    NOTES
        The returned array is NOT terminated.  Take care to use numPnts.
        This function assumes origin of (0, 0) is the center of the ellipse.
        This function will choose points along the major axis
            That is, (a > b) ? choose x : choose y
        This function iterates through the ellipse's (x, y) coordinates
            in the following order regardless of the major axis:
            (-a, 0), (0, b), (+a, 0), (0, -b), (-a + 1, y)
        Coordinate values chosen by this function are whole numbers
 */
double* plot_ellipse_points(double aVal, double bVal, int* numPnts);


/*
    PURPOSE - Determine the center coordinates of a rectangle given it's width
        and height.
    INPUT
        width - Width of the window, minimum 3
        height - Height of the window, minimum 3
        xCoord - Pointer to an int variable to store the "x" coordinate of the center
        yCoord - Pointer to an int variable to store the "y" coordinate of the center
        orientWin - MACRO representation of window orientation if ever the center isn't
            the perfect center.  Function defaults to HM_UP_LEFT if orientWin is
            invalid.
    OUTPUT
        On success, true;
        On failure, false;
 */
bool determine_center(int width, int height, int* xCoord, int* yCoord, int orientWin);


/*
    PURPOSE - Convert an array of point coordinates, stored as doubles, relative to
        center (centX, centY), into a linked list of hcCartCoord structs
    INPUT
        relEllipseCoords - A non-nul-terminated array of doubles.  Each even index
            is an x point and each odd index is a y point.  These x and y points
            make up coordinates.
        numPnts - The number of points contained in relEllipseCoords.  This value
            must be even and positive.
        centX - x value of the center coordinate
        centY - y value of the center coordinate
    OUTPUT
        On success, a pointer to the head node of a linked list of heap-allocated
            hcCartCoord structs translated from reEllipseCoords
        On failure, NULL
 */
hcCartCoord_ptr build_geometric_list(double* relEllipseCoords, int numPnts, int centX, int centY);


/*
    PURPOSE - Calculate the distance between integer representations of two points
    INPUT
        xCoord1 - X coordinate of point1
        yCoord1 - Y coordinate of point1
        xCoord2 - X coordinate of point2
        yCoord2 - Y coordinate of point2
    OUTPUT
        On success, the distance between point1 and point2 calculated to the greatest degree of precision
        On failure, 0
 */
double calc_int_point_dist(int xCoord1, int yCoord1, int xCoord2, int yCoord2);


/*
    PURPOSE - Calculate the slope of a line formed by two points
    INPUT
        xCoord1 - X coordinate of point1
        yCoord1 - Y coordinate of point1
        xCoord2 - X coordinate of point2
        yCoord2 - Y coordinate of point2
    OUTPUT
        The slope between point1 and point2 calculated to the greatest degree of precision
 */
double calc_int_point_slope(int xCoord1, int yCoord1, int xCoord2, int yCoord2);


/*
    PURPOSE - Verify that the slope between two points is equivalent to the given slope
    INPUT
        xCoord1 - X coordinate of point1
        yCoord1 - Y coordinate of point1
        xCoord2 - X coordinate of point2
        yCoord2 - Y coordinate of point2
        slope - Verify that point1 and point2 form this slope
        maxPrec - Maximum floating point precision when checking calculated slope against 'slope'
    OUTPUT
        If the calculated slope is "close enough" to the slope, true
        Otherwise, false
        On error, false
 */
bool verify_slope(int xCoord1, int yCoord1, int xCoord2, int yCoord2, double slope, int maxPrec);


/*
    PURPOSE - Calculate the center point of a line created by two points
    INPUT
        point1_ptr - Pointer to a hmLineLen struct holding the x and y coordinates of a point
        point2_ptr - Pointer to a hmLineLen struct holding the x and y coordinates of a point
        midPoint_ptr - Out parameter to hold the coordinates and ~length of the mid point between point1
            and point2
        rndDbl - The direction to round the resulting coordinates:
            Use the HM_* MACROS above or
            Use the original MACROS from fenv.h or
            Pass 0 to utilize the default settings
    OUTPUT
        On success, true
        On failure, false
    NOTES
        When successful, this function will store the coordinates and distance from point1_ptr into midPoint_ptr.
        This function will fail if the point1 node is the same as the point2 node
        This function will fail if the point1 coordinates are the same as the point2 coordinates
 */
bool determine_mid_point(hmLineLen_ptr point1_ptr, hmLineLen_ptr point2_ptr, hmLineLen_ptr midPoint_ptr, int rndDbl);


/*
    PURPOSE - Solve the point slope form of a line equation for a missing x coordinate, rounded using rndDbl
    INPUT
        knownX1 - The x coordinate of the known point
        knownY1 - The y coordinate of the known point
        knownY0 - The y coordinate associated with the x coordinate to solve for
        slope - The slope of the line
        rndDbl - The direction to round the resulting coordinates:
            Use the HM_* MACROS above or
            Use the original MACROS from fenv.h or
            Pass 0 to utilize the default settings
    OUTPUT
        On success, the x coordinate associated with knownY2 rounded in accordance with rndDbl
        On failure, 0
    NOTES
        The actual calculated result of this function is more likely to be a floating point value instead
            of a whole number.  This function was written to round the resulting double to an integer to
            aid in plotting points.
 */
int solve_point_slope_x(int knownX1, int knownY1, int knownY0, double slope, int rndDbl);


/*
    PURPOSE - Solve the point slope form of a line equation for a missing y coordinate, rounded using rndDbl
    INPUT
        knownX1 - The x coordinate of the known point
        knownY1 - The y coordinate of the known point
        knownX0 - The x coordinate associated with the x coordinate to solve for
        slope - The slope of the line
        rndDbl - The direction to round the resulting coordinates:
            Use the HM_* MACROS above or
            Use the original MACROS from fenv.h or
            Pass 0 to utilize the default settings
    OUTPUT
        On success, the y coordinate associated with knownY2 rounded in accordance with rndDbl
        On failure, 0
    NOTES
        The actual calculated result of this function is more likely to be a floating point value instead
            of a whole number.  This function was written to round the resulting double to an integer to
            aid in plotting points.
 */
int solve_point_slope_y(int knownX1, int knownY1, int knownX0, double slope, int rndDbl);


//////////////////////////////////////////////////////////////////////////////
////////////////////////// GEOMETRIC FUNCTIONS STOP //////////////////////////
//////////////////////////////////////////////////////////////////////////////

#endif  // __HARKLEMATH__
