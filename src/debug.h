/*
 * ****************************************************************************
 * Function and macro for debug management
 *
 * DEBUG MODE
 * There is different types of debug. Debug from specific type are displayed 
 * according to the current debug mode set. Debug mode is set from 
 * define constants which are:
 * 	DBG_ACTIVE -> Enable or disable dbg mode (If disable, no debug are displayed)
 * 	DBG_ERR_ACTIVE -> Active the error debugs
 * 	DBG_WARN_ACTIVE -> Active the warning debug
 * 	DBG_INFO_ACTIVE -> Active the information debugs
 *
 * SET VARIABLES
 * You can define the variables using define in this file or directly
 * from the gcc compiler by using 'gcc -VAR_TO_SET tocompile.c'
 *
 * Date:	Dec 12, 2015
 * Author:	Constantin MASSON <constantin.grinda@gmail.com>
 * ****************************************************************************
 */

//Set debug mode
#define DBG_ACTIVE
#define DBG_ERR_ACTIVE
#define DBG_WARN_ACTIVE
#define DBG_INFO_ACTIVE

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdarg.h> //For va_list

//Define the parameters for each dbg mode
#define DBG_ERR		stdout, __FILE__, __LINE__, "ERR"
#define DBG_WARN	stdout, __FILE__, __LINE__, "WARN"
#define DBG_INFO	stdout, __FILE__, __LINE__, "INFO"


// ****************************************************************************
// Function prototypes
// ****************************************************************************
void dbgPrint(FILE*, char*, int, char*, char*, ...);


// ****************************************************************************
// Debug macros
// ****************************************************************************
//If the debug mode is set, debug function are relevant and display something
#ifdef DBG_ACTIVE

/** @def dgbError display a debug message with a parameter */
#ifdef DBG_ERR_ACTIVE
#define dbgError(msg, ...) dbgPrint(DBG_ERR, msg, ##__VA_ARGS__)
#else
#define dbgError(msg, ...)
#endif

/** @def dbgWarn(x) Display a warning dbg message */
#ifdef DBG_WARN_ACTIVE
#define dbgWarn(msg, ...) dbgPrint(DBG_WARN, msg, ##__VA_ARGS__)
#else
#define dbgWarn(msg, ...)
#endif

/** @def dbgMessage(x) Display a simple dbg info message */
#ifdef DBG_INFO_ACTIVE
#define dbgInfo(msg, ...) dbgPrint(DBG_INFO, msg, ##__VA_ARGS__)
#else
#define dbgInfo(msg, ...)
#endif


// ****************************************************************************
// PART IF DEBUG MODE IS DISABLED
// ****************************************************************************
//If debug mode is not set, all dbg functions are disabled
#else
#define dbgError(msg, ...)
#define dbgWarn(msg, ...)
#define dbgInfo(msg, ...)

#endif //End debug_mode expression
#endif //General end ifndef




