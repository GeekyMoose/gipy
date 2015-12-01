/*
 * ****************************************************************************
 * Function and macro for error / debug management
 *
 * Error and debug are displayed according to the current set mod. 
 * 	DEBUG_MODE	-> Display all debug
 *
 * You can define the variable from the gcc compiler by using
 * 	gcc -DDEBUG_MODE tocompile.c
 * By default, any of these define are set (No debug displayed and all error 
 * displayed)
 *
 * Date:	Nov 29, 2015
 * Author:	Constantin MASSON <constantin.grinda@gmail.com>
 * ****************************************************************************
 */

#ifndef _ERRMAN_H_
#define _ERRMAN_H_

#include <stdio.h>
#include <stdarg.h> //For va_list

#define DEBUG_STREAM stdout //Default debug stream output
#define ERROR_STREAM stderr //Default error stream output


// ****************************************************************************
// General Macro
// ****************************************************************************

//If the debug mode is set, debug function are relevant and display something
#ifdef DEBUG_MODE

/**
 * @def dgbError(x)
 * Display a debug message with a parameter. 
 * IMPORTANT, x is a set of parameter following dgbPrint parameters therefore, 
 * in souce code, x must be surrounded by ()
 * For example, dbgError(("a message and int val=%d", val));
 */
#define dbgError(x) dbgPrint(DEBUG_STREAM, __FILE__, __LINE__, x)

/**
 * @def dbgMessage(x)
 * Display a simple dbg message
 */
#define dbgMessage(x) dbgPrint(DEBUG_STREAM, __FILE__, __LINE__, x)


//If debug mode is not set, all dbg functions are disabled
#else
#define dbgError(x)
#define dbgMessage(x)

#endif


// ****************************************************************************
// Functions
// ****************************************************************************
/**
 * @brief		Display an error on the stderr with special format
 *
 * @param pMsg	Message with specific format
 * @param ...	Print format
 * @return	void
 */
void printError(char *fmt, ...){
	va_list	args;
	va_start(args, fmt);
	fprintf(stderr, "[ERR]: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}

/**
 * @brief			Display a debug message
 * @details			This function is only used for debug, usually called by a debug macro 
 * 					NOTE: Usually, the third first parameters are formated with  
 * 					stdout, __FILE__, __LINE. IMPORTANT: we assume these 3 parameters 
 * 					are valid! (Skipp useless NULL check, it's a debug function, dont be 
 * 					naughty with :p)
 *
 * @param pStream	Output stream for dbg message
 * @param pFile		File name's where the debug was called
 * @param pLine		Line where dbg was called
 * @param pMsg		Message to display for this dbg
 * @param ...		Format for message
 * @return void
 */
void dbgPrint(FILE *pStream, char *pFile, int pLine, char *pMsg, ...){
	va_list args;
	va_start(args, pMsg);
	fprintf(pStream, "[DBG]:[%s:%d]-> ", pFile, pLine);
	vfprintf(pStream, pMsg, args);
	fprintf(pStream, "\n");
	va_end(args);
}



#endif



