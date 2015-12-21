/*
 * ****************************************************************************
 * Functions for debug management
 *
 * Date:	Dec 12, 2015
 * Author:	Constantin MASSON <constantin.grinda@gmail.com>
 * ****************************************************************************
 */

#include "debug.h"

// ****************************************************************************
// Debug Functions
// ****************************************************************************
/**
 * @brief			Display a debug message
 * @details			This function is only used for debug, usually called by a debug macro 
 * 					NOTE: Usually, the third first parameters are formated with  
 * 					stdout, __FILE__, __LINE. IMPORTANT: we assume these 3 parameters 
 * 					are valid! (It's a debug function, dont be naughty with :p)
 *
 * @param pStream	Output stream for dbg message
 * @param pFile		File name's where the debug was called
 * @param pLine		Line where dbg was called
 * @param pFlag		Message displayed just before dbg message
 * @param pMsg		Message to display for this dbg
 * @param ...		Format for message
 * @return void
 */
void dbgPrint(FILE *pStream, char *pFile, int pLine, char *pFlag, char *pMsg, ...){
	va_list args;
	va_start(args, pMsg);
	fprintf(pStream, "[%s]:[%s:%d]-> ", pFlag, pFile, pLine);
	vfprintf(pStream, pMsg, args);
	fprintf(pStream, "\n");
	va_end(args);
}


