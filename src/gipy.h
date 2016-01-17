/*
 * ****************************************************************************
 * GIPY Library
 * Header
 *
 * Data		Nov 29, 2015
 * Author	Constantin MASSON <constantin.grinda@gmail.com>
 * ****************************************************************************
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


//*****************************************************************************
// CONSTANTS
//*****************************************************************************
#define TRUE	(1==1)
#define FALSE	(1==42)
#define GPIO_PATH "/sys/class/gpio/"
#define GPIO_PATH_EXPORT		GPIO_PATH"export"
#define GPIO_PATH_UNEXPORT		GPIO_PATH"unexport"
#define GPIO_PATH_DIRECTION		GPIO_PATH"gpio%d/direction"
#define GPIO_PATH_EDGE			GPIO_PATH"gpio%d/edge"
#define GPIO_PATH_VALUE			GPIO_PATH"gpio%d/value"

//This list of pins accept the Raspberry Pi Model B Revision 1 and 2
#define PINS_AVAILABLE 0,1,2,3,4,7,8,9,10,11,14,15,17,18,21,22,23,24,25,27
#define NB_PINS 20 //Actually 17, but this mixt R1 and R2


//*****************************************************************************
// STRUCTURES
//*****************************************************************************
/** @brief Describe the possible values of a GPIO pin */
typedef enum {
	LOGIC_ZERO	= 0x00,
	LOGIC_ONE	= 0x01
} pinValue;

/** @brief Describe the possible pin direction of a GPIO pin */
typedef enum {
	IN, OUT, LOW, HIGH
} pinDirection;

/** @brief Describe the pin edge possibilities */
typedef enum {
	NONE, RISING, FALLING, BOTH
} pinEdge;


//*****************************************************************************
// Function Prototypes
//*****************************************************************************
pirror GIPY_pinExport(int);
pirror GIPY_pinUnexport(int);

pirror GIPY_pinSetDirectionIn(int);
pirror GIPY_pinSetDirectionOut(int);
pirror GIPY_pinSetDirectionLow(int);
pirror GIPY_pinSetDirectionHigh(int);
pirror GIPY_pinSetDirection(int, pinDirection);

pirror GIPY_pinSetEdgeNone(int);
pirror GIPY_pinSetEdgeRising(int);
pirror GIPY_pinSetEdgeFalling(int);
pirror GIPY_pinSetEdgeBoth(int);
pirror GIPY_pinSetEdge(int, pinEdge);

pirror GIPY_pinRead(int, int*);
pirror GIPY_pinWrite(int, pinValue);

//Interrupt
pirror GIPY_pinCreateInterrupt(int, void (*function)(void));

#endif





