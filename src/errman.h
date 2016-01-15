/*
 * ****************************************************************************
 * Function and macro for error management
 *
 *
 * Date:	Nov 29, 2015
 * Author:	Constantin MASSON <constantin.grinda@gmail.com>
 * ****************************************************************************
 */

#ifndef _ERRMAN_H_
#define _ERRMAN_H_

#include <stdio.h>
#include <stdarg.h> //For va_list


// ****************************************************************************
// Enum
// ****************************************************************************
/*
 * @brief Enum with all possible gipy error
 * GE for Gipy error (In case of you wonder, I had no better idee actually..)
 */
typedef enum {
	//General error
	GE_OK = 0, //No error
	GE_PERM, //Operation not permitted
	GE_NOENT, //No such file or directory
	GE_IO, //Input / Output error
	GE_PARAM, //Invalid parameters

	//Pin specific error
	GE_PIN, //Invalid pin number
	GE_PINDIR, //Invalid pin direction
	GE_PINVAL //Invalid pin value
} pirror;


// ****************************************************************************
// Prototypes
// ****************************************************************************
void printError(FILE *pStream, const char *fmt, ...);


#endif



