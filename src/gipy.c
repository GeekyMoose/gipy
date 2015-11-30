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
static int isValidPinNumber(int);
static void printError(char *, ...);


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
	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		printError("Invalid pin number: %d", pPin);
		return -1;
	}

	//Create the GPIO value path and try to open it
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH"gpio%d/value", pPin);
	int file = open(stamp, O_RDONLY);
	if(file < 1){
		printError("Unable to open the file: %s", stamp);
		return -1;
	}
	
	char buff = 'a';
	int n;
	if((n = read(file, &buff, 1)) == -1){
		close(file);
		printError("Unable to read from file: %s", stamp);
		return -1;
	}
	close(file);
	return buff-'0'; //n equals read value
}

/**
 * @brief	Write a value for a specific GPIO Pin
 * @detail	The pin must have been enabled before. If the value given is not 
 * 			valid, the process is stopped and return the error state
 *
 * @param	Pin number where to write
 * @param	Value to set to this pin (should be from pinValue enum)
 * @return	1 if successfully written, otherwise, return -1
 */
int GIPY_pinWrite(int pPin, pinValue pValue){
	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		printError("Invalid pin number: %d", pPin);
		return -1;
	}

	//check whether the pValue is valid
	if(pValue != LOGIC_ZERO && pValue != LOGIC_ONE){
		printError("Invalid value (%d) for pin: %d", pValue, pPin);
		return -1;
	}

	//Create the GPIO value path and try to open it
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH"gpio%d/value", pPin);
	int file = open(stamp, O_WRONLY);
	if(file < 1){
		printError("Unable to open the file: %s", stamp);
		return -1;
	}

	//Try to write the value in the GPIO value file
	char buff[2];
	sprintf(buff, "%d", pValue);
	if(write(file, buff, 2) != 2){
		printError("Unable to write the value %d in file %s", pValue, stamp);
		close(file);
		return -1;
	}
	close(file);
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
		close(file);
		return -1;
	}
	close(file);
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
static int isValidPinNumber(int pPin){
	static int validPins[NB_PINS] = {PINS_AVAILABLE}; //Init at first call
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
static void printError(char *fmt, ...){
	va_list	args;
	va_start(args, fmt);
	fprintf(stderr, "[ERR]: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}
