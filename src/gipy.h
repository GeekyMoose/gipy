/*
 * ****************************************************************************
 * GIPY Library
 * ****************************************************************************
 * Header
 *
 * @author	Constantin MASSON <constantin.grinda@gmail.com>
 * @data	Nov 29, 2015
 */


#ifndef _HEADER_GIPY_H_
#define _HEADER_GIPY_H_

//General include (External lib)
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "errman.h" //Error management


//*****************************************************************************
// CONSTANTS
//*****************************************************************************
#define TRUE	1
#define FALSE	0
#define GPIO_PATH "/home/caribou/Documents/w-work/data/sys/class/gpio/"

//Pin management
#define PINS_AVAILABLE 2,3,4,7,8,9,10,11,14,15,17,18,22,23,24,25,27
#define NB_PINS 17


//*****************************************************************************
// STRUCTURES
//*****************************************************************************
/** @brief Describe the possible values of a GPIO pin */
typedef enum {
	LOGIC_ZERO, LOGIC_ONE
} pinValue;

/** @brief Describe the possible pin direction of a GPIO pin */
typedef enum {
	IN, OUT, LOW, HIGH
} pinType;

/** @brief Describe the pin edge possibilities */
typedef enum {
	NONE, RISING, FALLING, BOTH
} pinEdge;


//*****************************************************************************
// MACRO - DEBUG MACRO
//*****************************************************************************

//*****************************************************************************
// Function Prototypes
//*****************************************************************************
int GIPY_pinRead(int);
int GIPY_pinWrite(int, pinValue);
int GIPY_pinEnable(int);
int GIPY_pinDisable(int);

// Private functions prototypes
static int isValidPinNumber(int);

#endif



