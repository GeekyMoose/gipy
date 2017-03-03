/*
 * -----------------------------------------------------------------------------
 * GIPY Library Header
 * Manage Raspberry Pi GPIO
 *
 * Since        Nov 29, 2015
 * Author:  Constantin MASSON <massonco@iro.umontreal.ca>
 * -----------------------------------------------------------------------------
 */

#ifndef _HEADER_GIPY_H_
#define _HEADER_GIPY_H_

//General include (External lib)
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <poll.h> //For interrupt thread
#include <pthread.h>
#include <stdint.h> //Used for pointer convert

#include "errman.h" //Error management
#include "debug.h" //Debug lib


//------------------------------------------------------------------------------
// CONSTANTS
//------------------------------------------------------------------------------
#define TRUE                    (1==1)
#define FALSE                   (1==42)
#define GPIO_PATH               "/sys/class/gpio/"
#define GPIO_PATH_EXPORT        GPIO_PATH"export"
#define GPIO_PATH_UNEXPORT      GPIO_PATH"unexport"
#define GPIO_PATH_DIRECTION     GPIO_PATH"gpio%d/direction"
#define GPIO_PATH_EDGE          GPIO_PATH"gpio%d/edge"
#define GPIO_PATH_VALUE         GPIO_PATH"gpio%d/value"

//This list of pins accept the Raspberry Pi Model B Revision 1 and 2
#define PINS_AVAILABLE 0,1,2,3,4,7,8,9,10,11,14,15,17,18,21,22,23,24,25,27
#define NB_PINS 20 //Actually 17, but this mixt R1 and R2


//------------------------------------------------------------------------------
// STRUCTURES
//------------------------------------------------------------------------------

/**
 * \brief Describe the possible values of a GPIO pin
 */
typedef enum {
    LOGIC_ZERO  = 0x00,
    LOGIC_ONE   = 0x01
} pinValue;

/**
 * \brief Describe the possible pin direction of a GPIO pin
 */
typedef enum {
    IN,
    OUT,
    LOW,
    HIGH
} pinDirection;

/**
 * \brief Describe the pin edge possibilities
 */
typedef enum {
    NONE,
    RISING,
    FALLING,
    BOTH
} pinEdge;


//------------------------------------------------------------------------------
// PROTOTYPES: Pin set direction functions
//------------------------------------------------------------------------------
/**
 * \brief           Export (Enable) a GPIO Pin.
 *
 * \param pPin      Pin number to enable
 * \return GE_OK    If no error
 * \return GE_PIN   If pin is not valid
 * \return GE_PERM  If unable to open export (or value) file
 * \return GE_IO    If unable to read in export file
 */
pirror GIPY_pinExport(int);

/**
 * \brief           Unexport (disable) a pin
 *
 * \param pPin      Pin to disable
 * \return GE_OK    If no error
 * \return GE_PIN   If pin is not valid
 * \return GE_PERM  If unable to open unexport file
 * \return GE_IO    If unable to write in export file
 */
pirror GIPY_pinUnexport(int);

/**
 * \brief               Set the pin direction to In 
 *
 * \param pPin          Pin to set
 * \return GE_OK        If no error
 * \return GE_PIN       If pin number is not valid
 * \return GE_PERM      If pin not exported
 * \return GE_NOENT     If unable to open (Write) dir file
 * \return GE_PINDIR    If the pin direction is not valid
 * \return GE_IO        If unable to write in direction file
 */
pirror GIPY_pinSetDirectionIn(int);

/**
 * \brief               Set the pin direction to Out
 *
 * \param pPin          Pin to set
 * \return GE_OK        If no error
 * \return GE_PIN       If pin number is not valid
 * \return GE_PERM      If pin not exported
 * \return GE_NOENT     If unable to open (Write) dir file
 * \return GE_PINDIR    If the pin direction is not valid
 * \return GE_IO        If unable to write in direction file
 */
pirror GIPY_pinSetDirectionOut(int);

/**
 * \brief               Set the pin direction to low
 *
 * \param pPin          Pin to set
 * \return GE_OK        If no error
 * \return GE_PIN       If pin number is not valid
 * \return GE_PERM      If pin not exported
 * \return GE_NOENT     If unable to open (Write) dir file
 * \return GE_PINDIR    If the pin direction is not valid
 * \return GE_IO        If unable to write in direction file
 */
pirror GIPY_pinSetDirectionLow(int);

/**
 * \brief               Set the pin direction to high
 *
 * \param pPin          Pin to set
 * \return GE_OK        If no error
 * \return GE_PIN       If pin number is not valid
 * \return GE_PERM      If pin not exported
 * \return GE_NOENT     If unable to open (Write) dir file
 * \return GE_PINDIR    If the pin direction is not valid
 * \return GE_IO        If unable to write in direction file
 */
pirror GIPY_pinSetDirectionHigh(int);

/**
 * \brief               Set the pin direction
 * \details             If direction is not valid, nothing is done
 *
 * \param pPin          Pin to set
 * \param pPinDir       Direction to set (From PinDirection enum)
 * \return GE_OK        If no error
 * \return GE_PIN       If pin number is not valid
 * \return GE_PERM      If pin not exported
 * \return GE_NOENT     If unable to open (Write) dir file
 * \return GE_PINDIR    If the pin direction is not valid
 * \return GE_IO        If unable to write in direction file
 */
pirror GIPY_pinSetDirection(int, pinDirection);


//------------------------------------------------------------------------------
// PROTOTYPES: GPIO set edge functions
//------------------------------------------------------------------------------

/**
 * \brief           Set edge of the given pin to None
 *
 * \param pPin      Pin to set
 * \return GE_OK    If no error
 * \return GE_PIN   If invalid pin
 * \return GE_PERM  If pin is not already exported
 * \return GE_NOENT If edge file unreachable
 * \return GE_IO    If unable to read edge file
 */
pirror GIPY_pinSetEdgeNone(int);

/**
 * \brief           Set edge of the given pin to Rising
 *
 * \param pPin      Pin to set
 * \return GE_OK    If no error
 * \return GE_PIN   If invalid pin
 * \return GE_PERM  If pin is not already exported
 * \return GE_NOENT If edge file unreachable
 * \return GE_IO    If unable to read edge file
 */
pirror GIPY_pinSetEdgeRising(int);

/**
 * \brief           Set edge of the given pin to falling
 *
 * \param pPin      Pin to set
 * \return GE_OK    If no error
 * \return GE_PIN   If invalid pin
 * \return GE_PERM  If pin is not already exported
 * \return GE_NOENT If edge file unreachable
 * \return GE_IO    If unable to read edge file
 */
pirror GIPY_pinSetEdgeFalling(int);

/**
 * \brief           Set edge of the given pin to both
 *
 * \param pPin      Pin to set
 * \return GE_OK    If no error
 * \return GE_PIN   If invalid pin
 * \return GE_PERM  If pin is not already exported
 * \return GE_NOENT If edge file unreachable
 * \return GE_IO    If unable to read edge file
 */
pirror GIPY_pinSetEdgeBoth(int);

/**
 * \brief           Set the edge for a given pin
 * \detail          If the given edge is not valid, edge 'none' will be set
 *
 * \param pPin      Pin to set
 * \param pEdge     Edge to set (From pinEdge enum)
 * \return GE_OK    If no error
 * \return GE_PIN   If invalid pin
 * \return GE_PERM  If pin is not already exported
 * \return GE_NOENT If edge file unreachable
 * \return GE_IO    If unable to read edge file
 */
pirror GIPY_pinSetEdge(int, pinEdge);

//------------------------------------------------------------------------------
// PROTOTYPES: GPIO Read / Write functions
//------------------------------------------------------------------------------

/**
 * \brief           Read a pin value
 *
 * \param pPin      Pin number to read
 * \param pRead     Pointer toward read value to fill
 * \return GE_OK    If no error
 * \return GE_PIN   If pin is not valid
 * \return GE_PERM  If pin is not exported
 * \return GE_IO    If unable to read from value file
 */
pirror GIPY_pinRead(int, int*);

/**
 * \brief               Write a value for a specific GPIO Pin
 * \detail              The pin must have been enabled before. 
 *                      Nothing done is invalid value given
 *
 * \param pPin          Pin number where to write
 * \param pValue        Value to set to this pin (should be from pinValue enum)
 * \return GE_OK        If no error
 * \return GE_PIN       If pin number is not valid
 * \return GE_PINVAL    If pin value is not valid
 * \return GE_IO        If unable to read from value file
 */
pirror GIPY_pinWrite(int, pinValue);


//------------------------------------------------------------------------------
// PROTOTYPES: interrupt functions
//------------------------------------------------------------------------------

/**
 * \brief               Create an interrupt for specific pin
 * \details             At most one interrupt can be created for a pin
 *                      Attention: if this pin already got an interrupt set, 
 *                      it will be lost and replaced by this new one.
 *
 * \param               pin linked with interrupt
 * \param               function to execut if interrupt generated
 * \return GE_OK        If no error
 * \return GE_PIN       If invalid pin number
 * \return GE_PERM      If pin not exported
 */
pirror GIPY_pinCreateInterrupt(int, void (*function)(void));

#endif



