/*
 * ****************************************************************************
 * GIPY Library
 * Manage Raspberry Pi GPIO
 *
 * Data:	Nov 29, 2015
 * Author:	Constantin MASSON <constantin.grinda@gmail.com>
 * ****************************************************************************
 */

#include "gipy.h"


//*****************************************************************************
// GPIO Export / Unexport functions
//*****************************************************************************
/**
 * @brief		Enable a GPIO Pin.
 *
 * @param		int the pin int number value
 * @return		1 if successfully enabled otherwise, return -1
 */
int GIPY_pinEnable(int pPin){
	dbgMessage(("Try to enable pin %d", pPin));

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError(("Invalid pin number: %d", pPin));
		printError("Unable to enable the pin %d: invalid pin number", pPin);
		return -1;
	}

	//Open the export sys file, check if successfully opened
	int file = open(GPIO_PATH"/export", O_WRONLY);
	if(file == -1){
		dbgErrorr(("Unreachable export file: "GPIO_PATH"/export"));
		printError("Unable to enable the pin %d", pPin);
		return -1;
	}

	//Write into file that this pin is set
	char tamp[3];
	sprintf(tamp, "%d", pPin);
	if(write(file, tamp, 3) != 3){
		dbgError(("Unable to write %d in "GPIO_PATH"/export", pPin));
		printError("Unable to enable the pin %d", pPin);
		close(file);
		return -1;
	}
	close(file);
	return 1;
}

/**
 * @brief		Disable a pin
 *
 * @param pPin	Pin to disable
 * @return		1 if successfully disabled, otherwise, return -1
 */
int GIPY_pinDisable(int pPin){
	dbgMessage(("Try to disable pin %d", pPin));

	//Check whether pin number is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError((BDG_PARAMS, "Invalid pin number: %d", pPin));
		printError("Unable to disable pin %d! Invalid pin", pPin);
		return -1;
	}

	//Try to open unexport file
	int file = open(GPIO_PATH"/unexport", WRONLY);
	if(file<0){
		dbgError(("Unreachable file: %s", GPIO_PATH"/unexport"));
		printError("Unable to disable the pin %d", pPin);
		return -1;
	}

	//Write into file that this pin is set
	char tamp[3];
	sprintf(tamp, "%d", pPin);
	if(write(file, tamp, 3) != 3){
		dbgError(("Unable to write %d in "GPIO_PATH"/unexport", pPin));
		printError("Unable to disable the pin %d", pPin);
		close(file);
		return -1;
	}
	close(file);
	return 1;
}


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
	dbgMessage((DBG_PARAMS, "Try to read pin %d", pPIn));

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError((DBG_PARAMS, "Invalid pin number: %d", pPin)); //Deleted if debug not set
		printError("Invalid pin number: %d", pPin);
		return -1;
	}

	//Create the GPIO value path and try to open it
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH"gpio%d/value", pPin);
	int file = open(stamp, O_RDONLY);
	if(file < 1){
		dbgError((DBG_PARAMS, "Unable to open the file: %s", stamp));
		printError("Unable to open the file: %s", stamp);
		return -1;
	}
	
	//Read from the file
	char buff;
	int n;
	if((n = read(file, &buff, 1)) == -1){
		close(file);
		dbgError((DBG_PARAMS, "Unable to read from file: %s", stamp));
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
	dbgMessage((DBG_PARAMS, "Try to write in pin %d", pPIn));

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError((DBG_PARAMS, "Invalid pin number: %d", pPin));
		printError("Invalid pin number: %d", pPin);
		return -1;
	}

	//check whether the pValue is valid
	if(pValue != LOGIC_ZERO && pValue != LOGIC_ONE){
		dbgError((DBG_PARAMS, "Invalid value (%d) for pin: %d", pValue, pPin));
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
// Tools functions
//*****************************************************************************
/**
 * @brief	Check whether the pin number is valid
 *
 * @param	int the pin number
 * @return	1 if valid, otherwise, return -1
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

