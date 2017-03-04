
 * -----------------------------------------------------------------------------
 * Function for error management and display
 * All the possible error are set from pirror
 *
 * Since:   Nov 29, 2015
 * Author:  Constantin MASSON
 * -----------------------------------------------------------------------------
 */

#include "errman.h"


// -----------------------------------------------------------------------------
// Error functions
// -----------------------------------------------------------------------------

void printError(FILE *pStream, const char *fmt, ...){
    if(pStream == NULL){
        pStream = stderr;
    }
    va_list args;
    va_start(args, fmt);
    fprintf(pStream, "[ERR]: ");
    vfprintf(pStream, fmt, args);
    fprintf(pStream, "\n");
    va_end(args);
}

