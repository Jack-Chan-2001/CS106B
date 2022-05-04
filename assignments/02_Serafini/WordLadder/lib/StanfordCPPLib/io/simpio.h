/*
 * File: simpio.h
 * --------------
 * This file exports a set of functions that simplify input/output
 * operations in C++ and provide some error-checking on console input.
 * 
 * @version 2016/09/29
 * - added getDouble method
 * @version 2015/07/05
 * - increased visibility of appendSpace function used by various IO
 *   prompting functions (no longer static)
 * @version 2014/10/19
 * - alphabetized functions
 * - converted many funcs to take const string& rather than string for efficiency
 */

#ifndef _simpio_h
#define _simpio_h

#include <iostream>
#include <string>

/*
 * Function: appendSpace
 * ---------------------
 * Adds a space at the end of the given string by reference if none is present.
 */
void appendSpace(std::string& prompt);

/*
 * Function: getChar
 * Usage: char c = getChar(prompt);
 * --------------------------------
 * Reads a complete line from <code>cin</code>, expecting the user to type a
 * single character. If this is the case, that character value is returned.
 * If the user types anything other than a one-character line of input,
 * the user is given a chance to reenter the value.
 *
 * If supplied, the optional <code>prompt</code> string is printed before
 * reading the value.
 *
 * The also optional <code>reprompt</code> argument provides an output message
 * displayed each time if the user types a file that is not found.
 * If no value is passed, defaults to, "You must type a single character. Try again.".
 */
char getChar(const std::string& prompt = "",
             const std::string& reprompt = "");

/*
 * Function: getDouble
 * Usage: double x = getDouble(prompt);
 * ------------------------------------
 * Equivalent to getReal.
 *
 * Reads a complete line from <code>cin</code> and scans it as a
 * floating-point number. If the scan succeeds, the floating-point
 * value is returned.  If the input is not a legal number or if
 * extraneous characters (other than whitespace) appear in the string,
 * the user is given a chance to reenter the value.
 *
 * If supplied, the optional <code>prompt</code> string is printed before
 * reading the value.
 *
 * The also optional <code>reprompt</code> argument provides an output message
 * displayed each time if the user types a file that is not found.
 * If no value is passed, defaults to, "Illegal numeric format. Try again.".
 */
double getDouble(const std::string& prompt = "",
                 const std::string& reprompt = "");

/*
 * Function: getDoubleBetween
 * Usage: double x = getDoubleBetween(prompt, min, max);
 * -----------------------------------------------------
 * Equivalent to getRealBetween.
 *
 * Reads a complete line from <code>cin</code> and scans it as a
 * floating-point number. If the scan succeeds, the floating-point
 * value is returned.  If the input is not a legal number or if
 * it is not between min and max, or if extraneous characters
 * (other than whitespace) appear in the string,
 * the user is given a chance to reenter the value.
 *
 * The required <code>prompt</code> string is printed before reading the value.
 */
double getDoubleBetween(const std::string& prompt, double min, double max);


/*
 * Function: getInteger
 * Usage: int n = getInteger(prompt);
 * ----------------------------------
 * Reads a complete line from <code>cin</code> and scans it as an
 * integer. If the scan succeeds, the integer value is returned. If
 * the argument is not a legal integer or if extraneous characters
 * (other than whitespace) appear in the string, the user is given
 * a chance to reenter the value.
 *
 * If supplied, the optional <code>prompt</code> string is printed before
 * reading the value.
 *
 * The also optional <code>reprompt</code> argument provides an output message
 * displayed each time if the user types a file that is not found.
 * If no value is passed, defaults to, "Illegal integer format. Try again.".
 */
int getInteger(const std::string& prompt = "",
               const std::string& reprompt = "");

/*
 * Function: getIntegerBetween
 * Usage: int n = getIntegerBetween(prompt, min, max);
 * ---------------------------------------------------
 * Reads a complete line from <code>cin</code> and scans it as an
 * integer. If the scan succeeds, the integer value is returned. If
 * the argument is not a legal integer, if it is not between min and max,
 * or if extraneous characters (other than whitespace) appear in the string,
 * the user is given a chance to reenter the value.
 *
 * The required <code>prompt</code> string is printed before reading the value.
 */
int getIntegerBetween(const std::string& prompt, int min, int max);

/*
 * Function: getLine
 * Usage: string line = getLine(prompt);
 * -------------------------------------
 * Reads a line of text from <code>cin</code> and returns that line
 * as a string.  The newline character that terminates the input is
 * not stored as part of the return value.  If supplied, the optional
 * <code>prompt</code> string is printed before reading the value.
 */
std::string getLine(const std::string& prompt = "");

/*
 * Alternate version of getLine that accepts a prompt and fills a given output
 * variable with its result.
 */
void getLine(const std::string& prompt,
             std::string& out);

/*
 * Alternate version of getLine that accepts an input stream to read from.
 * Meant as a drop-in replacement for the standard C++ getline (lowercase L)
 * function.
 */
void getLine(std::istream& input,
             std::string& out);

/*
 * Function: getReal
 * Usage: double x = getReal(prompt);
 * ----------------------------------
 * Equivalent to getDouble.
 *
 * Reads a complete line from <code>cin</code> and scans it as a
 * floating-point number. If the scan succeeds, the floating-point
 * value is returned.  If the input is not a legal number or if
 * extraneous characters (other than whitespace) appear in the string,
 * the user is given a chance to reenter the value.
 *
 * If supplied, the optional <code>prompt</code> string is printed before
 * reading the value.
 *
 * The also optional <code>reprompt</code> argument provides an output message
 * displayed each time if the user types a file that is not found.
 * If no value is passed, defaults to, "Illegal numeric format. Try again.".
 */
double getReal(const std::string& prompt = "",
               const std::string& reprompt = "");

/*
 * Function: getRealBetween
 * Usage: double x = getRealBetween(prompt, min, max);
 * ---------------------------------------------------
 * Equivalent to getDoubleBetween.
 *
 * Reads a complete line from <code>cin</code> and scans it as a
 * floating-point number. If the scan succeeds, the floating-point
 * value is returned.  If the input is not a legal number or if
 * it is not between min and max, or if extraneous characters
 * (other than whitespace) appear in the string,
 * the user is given a chance to reenter the value.
 *
 * The required <code>prompt</code> string is printed before reading the value.
 */
double getRealBetween(const std::string& prompt, double min, double max);

/*
 * Reads a complete line from <code>cin</code> and treats it as a
 * yes-or-no answer to a question.  Returns <code>true</code> if the line
 * typed begins with a 'y' or 'Y', and returns <code>false</code> if it begins
 * with a 'n' or 'N'.  Otherwise the user is given a chance to reenter the
 * value.
 *
 * If supplied, the optional <code>prompt</code> string is printed before
 * reading the value.
 *
 * The also optional <code>reprompt</code> argument provides an output message
 * displayed each time if the user types a file that is not found.
 * If no value is passed, defaults to,
 * "Please type a word that starts with 'Y' or 'N'.".
 * 
 * The also also optional <code>defaultValue</code> argument indicates what
 * should happen if the user just presses Enter rather than typing Y or N.
 * By default, the user is re-prompted, but if a defaultValue is passed here,
 * pressing Enter will be equivalent to having typed that value.
 * This is useful where the default Y/N answer is Yes or No and you want to
 * let the user avoid typing.
 */
bool getYesOrNo(const std::string& prompt = "",
                const std::string& reprompt = "",
                const std::string& defaultValue = "");

#include <private/init.h>   // ensure that Stanford C++ lib is initialized

#endif // _simpio_h
