/*
 * ****************************************************************************
 * GIPY Library
 * ****************************************************************************
 * Manage Raspberry Pi GPIO
 *
 * @data	Nov 29, 2015
 * @author	Constantin MASSON <constantin.grinda@gmail.com>
 */

#include "gipy.h"
#include <stdarg.h> //For the printError() function

// Private functions prototypes
int isValidPinNumber(int);
void printError(char *, ...);


//*****************************************************************************
// GPIO Acces Functions
//*****************************************************************************
/**
 * @brief	Read the value from a specific pin
 *
 * @param	int the pin number where to read
 * @return	an int corresponding to the pin state (0 or 1)
 */
int GIPY_pinRead(int pPin){
	//#define GPIO_PIN_VAL_FILE(S,P)	sprintf(S, "%sgpio%d/value", GPIO_PATH, P)
	return 1;
}

/**
 * @brief	Write a value for a specific GPIO Pin
 * @detail	The pin must have been enabled before!
 *
 * @param	Pin number where to write
 * @param	Value to set to this pin (From pinValue enum)
 * @return	1 if successfully written, otherwise, return -1
 */
int GIPY_pinWrite(int pPin, pinValue pValue){
	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		printError("Invalid pin number: %d", pPin);
		return -1;
	}

	//Create the GPIO value path and try to open it
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH"%dgpio/value", pPin);
	int file = open(stamp, O_WRONLY);
	if(file < 1){
		printError("Unable to open the file: %s", stamp);
		return -1;
	}

	//Try to write the value in the GPIO value file
	//write(stamp, "%d", pValue);
	return 1;
}


//*****************************************************************************
// GPIO Configuration Functions
//*****************************************************************************
/**
 * @brief		Enable a GPIO Pin.
 *
 * @param		int the pin int number value
 * @return		1 if successfully enabled otherwise, return -1
 */
int GIPY_pinEnable(int pPin){
	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		printError("Invalid pin number: %d", pPin);
		return -1;
	}

	//Open the export sys file, check if successfully opened
	int file = open(GPIO_PATH "/export", O_WRONLY);
	if(file == -1){
		printError("Unable to open the export file: "GPIO_PATH"/export");
		return -1;
	}

	//Write into file that this pin is set
	char tamp[3];
	sprintf(tamp, "%d", pPin);
	if(write(file, tamp, 3) != 3){
		printError("Unable to write the value in the file for pin %d", pPin);
		return -1;
	}
	return 1;
}


//*****************************************************************************
// Tools functions
//*****************************************************************************
/**
 * @brief Check whether the pin number is valid
 *
 * @param int the pin number
 * @return 1 if valid, otherwise, return -1
 */
int isValidPinNumber(int pPin){
	int validPins[NB_PINS] = {PINS_AVAILABLE};
	int k = 0;
	for(k=0; k<NB_PINS; k++){
		if(validPins[k] == pPin){
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @brief display an error on the stderr with special format
 *
 * @param message with specific format
 */
void printError(char *fmt, ...){
	va_list	args;
	va_start(args, fmt);
	fprintf(stderr, "[ERR]: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}


