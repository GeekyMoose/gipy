/*
 * -----------------------------------------------------------------------------
 * Functions for debug management
 *
 * Since:   Dec 12, 2015
 * Author:  Constantin MASSON
 * -----------------------------------------------------------------------------
 */

#include "debug.h"

// -----------------------------------------------------------------------------
// Debug Functions
// -----------------------------------------------------------------------------
void dbgPrint(FILE *pStream, char *pFile, int pLine, char *pFlag, char *pMsg, ...){
    va_list args;
    va_start(args, pMsg);
    fprintf(pStream, "[%s]:[%s:%d]-> ", pFlag, pFile, pLine);
    vfprintf(pStream, pMsg, args);
    fprintf(pStream, "\n");
    va_end(args);
}


