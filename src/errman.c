/*
 * ****************************************************************************
 * Function for error management and display
 * All the possible error are set from pirror
 *
 * Date:	Nov 29, 2015
 * Author:	Constantin MASSON <constantin.grinda@gmail.com>
 * ****************************************************************************
 */

#include "errman.h"


// ****************************************************************************
// Error functions
// ****************************************************************************
/**
 * @brief			Display an error on the stderr with special format
 *
 * @param pStream	Output stream where to display error (Default used if NULL given)
 * @param pMsg		Message with specific format
 * @param ...		Print format
 * @return			void
 */
void printError(FILE *pStream, const char *fmt, ...){
	if(pStream == NULL){
		pStream = stderr;
	}
	va_list	args;
	va_start(args, fmt);
	fprintf(pStream, "[ERR]: ");
	vfprintf(pStream, fmt, args);
	fprintf(pStream, "\n");
	va_end(args);
}

