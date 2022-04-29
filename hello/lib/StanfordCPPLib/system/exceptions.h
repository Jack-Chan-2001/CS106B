/*
 * File: exceptions.h
 * ------------------
 * This file contains a top-level exception handler to print exceptions thrown
 * by student code on the console.
 * It also contains some wizardry to try to retrieve a stack trace when the
 * exception is thrown, though it is hard to consistently do this on all platforms.
 * 
 * @author Marty Stepp
 * @version 2016/11/07
 * - added cleanupFunctionNameForStackTrace
 * @version 2016/10/30
 * - moved recursion functions to recursion.h/cpp
 * @version 2014/11/12
 * - made printStackTrace function publicly available
 * - added top-level signal handler (for null-pointer derefs etc.)
 * @since 2014/11/05
 */

#ifndef _exceptions_h
#define _exceptions_h

#include <iostream>

namespace exceptions {
/*
 * Strips some extraneous text from a function's name/header to make it look
 * better when printed in a stack trace.
 * e.g. basic_string -> string, removes std::, removes some weird compiler gibberish.
 */
std::string cleanupFunctionNameForStackTrace(std::string function);

/*
 * Called by C++ lib's main wrapper so that the stack trace knows the program's name.
 * (Taken from argv[0].)
 */
std::string& getProgramNameForStackTrace();

/*
 * Returns whether the top-level exception handler is enabled.
 * Initially false.
 */
bool getTopLevelExceptionHandlerEnabled();

/*
 * Prints a stack trace to the given output stream.
 * Defaults to the system standard error console (cerr).
 * (Stack traces are highly OS- and compiler-dependent, so this function
 *  may not work perfectly on all platforms.  It has been tested to work
 *  on Linux with GCC/G++, Mac OS X with clang++, and Windows with MinGW.)
 */
void printStackTrace();
void printStackTrace(std::ostream& out);

/*
 * Called by C++ lib's main wrapper so that the stack trace knows the program's name.
 * (Taken from argv[0].)
 */
void setProgramNameForStackTrace(char* programName);

/*
 * Sets whether the top-level exception handler is enabled.
 */
void setTopLevelExceptionHandlerEnabled(bool enabled);

/*
 * Whether the given function should be filtered out when displaying a stack trace.
 * Not meant to be called by clients.
 */
bool shouldFilterOutFromStackTrace(const std::string& function);
}

// free functions

#include <private/init.h>   // ensure that Stanford C++ lib is initialized

#endif // _exceptions_h
