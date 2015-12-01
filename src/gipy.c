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
#include "errman.h"


//*****************************************************************************
// GPIO Export / Unexport functions
//*****************************************************************************
/**
 * @brief		Enable a GPIO Pin.
 *
 * @param pPin	int the pin int number value
 * @return		1 if successfully enabled otherwise, return -1
 */
int GIPY_pinEnable(int pPin){
	dbgMessage((DBG_PARAMS, "[START] Try to enable pin %d", pPin));

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError((DBG_PARAMS, "Invalid pin number: %d", pPin));
		printError("Unable to enable the pin %d: invalid pin number", pPin);
		return -1;
	}

	//Open the export sys file, check if successfully opened
	int file = open(GPIO_PATH"/export", O_WRONLY);
	if(file == -1){
		dbgError((DBG_PARAMS, "Unreachable export file: "GPIO_PATH"/export"));
		printError("Unable to enable the pin %d", pPin);
		return -1;
	}

	//Write into file that this pin is set
	char tamp[3];
	sprintf(tamp, "%d", pPin);
	if(write(file, tamp, 3) != 3){
		dbgError((DBG_PARAMS, "Unable to write %d in "GPIO_PATH"/export", pPin));
		printError("Unable to enable the pin %d", pPin);
		close(file);
		return -1;
	}
	close(file);
	dbgMessage((DBG_PARAMS, "[SUCCESS] Success: pin %d enabled", pPin));
	return 1;
}

/**
 * @brief		Disable a pin
 *
 * @param pPin	Pin to disable
 * @return		1 if successfully disabled, otherwise, return -1
 */
int GIPY_pinDisable(int pPin){
	dbgMessage((DBG_PARAMS, "[START] Try to disable pin %d", pPin));

	//Check whether pin number is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError((DBG_PARAMS, "Invalid pin number: %d", pPin));
		printError("Unable to disable pin %d! Invalid pin", pPin);
		return -1;
	}

	//Try to open unexport file
	int file = open(GPIO_PATH"/unexport", O_WRONLY);
	if(file<0){
		dbgError((DBG_PARAMS, "Unreachable file: %s", GPIO_PATH"/unexport"));
		printError("Unable to disable the pin %d", pPin);
		return -1;
	}

	//Write into file that this pin is set
	char tamp[3];
	sprintf(tamp, "%d", pPin);
	if(write(file, tamp, 3) != 3){
		dbgError((DBG_PARAMS, "Unable to write %d in "GPIO_PATH"/unexport", pPin));
		printError("Unable to disable the pin %d", pPin);
		close(file);
		return -1;
	}
	close(file);
	dbgMessage((DBG_PARAMS, "[SUCCESS] pin %d disabled", pPin));
	return 1;
}


//*****************************************************************************
// Pin set direction functions
//*****************************************************************************
/**
 * @brief			Set the pin direction to In 
 *
 * @param pPin		Pin to set
 * @return			1 if successfully set, otherwise, return -1
 */
int GIPY_pinSetDirectionIn(int pPin){
	return GIPY_pinSetDirection(pPin, IN);
}

/**
 * @brief			Set the pin direction to Out
 *
 * @param pPin		Pin to set
 * @return			1 if successfully set, otherwise, return -1
 */
int GIPY_pinSetDirectionOut(int pPin){
	return GIPY_pinSetDirection(pPin, OUT);
}

/**
 * @brief			Set the pin direction to low
 *
 * @param pPin		Pin to set
 * @return			1 if successfully set, otherwise, return -1
 */
int GIPY_pinSetDirectionLow(int pPin){
	return GIPY_pinSetDirection(pPin, LOW);
}

/**
 * @brief			Set the pin direction to high
 *
 * @param pPin		Pin to set
 * @return			1 if successfully set, otherwise, return -1
 */
int GIPY_pinSetDirectionHigh(int pPin){
	return GIPY_pinSetDirection(pPin, HIGH);
}

/**
 * @brief			Set the pin direction
 * @details			If direction is not valid, nothing is done
 *
 * @param pPin		Pin to set
 * @param pPinDir	Direction to set (From PinDirection enum)
 * @return			1 if successfully set, otherwise, return -1
 */
int GIPY_pinSetDirection(int pPin, pinDirection pPinDir){
	dbgMessage((DBG_PARAMS, "[START] Try to change direction pin %d to %d", pPin, pPinDir));

	//Check whether pin number is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError((DBG_PARAMS, "Invalid pin number: %d", pPin));
		printError("Unable to set pin %d direction! Invalid pin", pPin);
		return -1;
	}

	//Open direction folder
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH"/gpio%d/direction", pPin);
	int file = open(stamp, O_WRONLY);
	if(file<1){
		dbgError((DBG_PARAMS, "Unreachable file %s for pin %d", stamp, pPin));
		printError("Unable to set pin %d direction!", pPin);
		return -1;
	}

	//Try to write the new direction in file according to dir parameter
	int writeError;
	switch(pPinDir){
		case IN:
			writeError = (write(file, "in", 3) == 3) ? 1 : -1;
			break;
		case OUT:
			writeError = (write(file, "out", 4) == 4) ? 1 : -1;
			break;
		case LOW:
			writeError = (write(file, "low", 3) == 3) ? 1 : -1;
			break;
		case HIGH:
			writeError = (write(file, "high", 5) == 5) ? 1 : -1;
			break;
		//Default mean the pin dir is not valid
		default:
			writeError = -1;
			break;
	}

	//Check the write process was successfully done
	if(writeError != 1){
		close(file);
		dbgError((DBG_PARAMS, "Unable to write in %s with value %d", stamp, pPinDir));
		printError("Unable to set pin %d direction!", pPin);
		return -1;
	}
	close(file);
	dbgMessage((DBG_PARAMS, "[SUCCESS] Direction pin %d is now %d", pPin, pPinDir));
	return 1;
}


// ****************************************************************************
// GPIO set edge functions
// ****************************************************************************

/**
 * @brief			Set edge of the given pin to None
 *
 * @param pPin		Pin to set
 * @return			1 if successfully set, otherwise, return -1
 */
int GIPY_pinSetEdgeNone(int pPin){
	return GIPY_pinSetEdge(pPin, NONE);
}

/**
 * @brief			Set edge of the given pin to Rising
 *
 * @param pPin		Pin to set
 * @return			1 if successfully set, otherwise, return -1
 */
int GIPY_pinSetEdgeRising(int pPin){
	return GIPY_pinSetEdge(pPin, RISING);
}

/**
 * @brief			Set edge of the given pin to falling
 *
 * @param pPin		Pin to set
 * @return			1 if successfully set, otherwise, return -1
 */
int GIPY_pinSetEdgeFalling(int pPin){
	return GIPY_pinSetEdge(pPin, FALLING);
}

/**
 * @brief			Set edge of the given pin to both
 *
 * @param pPin		Pin to set
 * @return			1 if successfully set, otherwise, return -1
 */
int GIPY_pinSetEdgeBoth(int pPin){
	return GIPY_pinSetEdge(pPin, BOTH);
}

/**
 * @brief			Set the edge for a given pin
 * @detail			If the given edge is not valid, edge 'none' will be set
 *
 * @param pPin		Pin to set
 * @param pEdge		Edge to set (From pinEdge enum)
 * @return			1 if successfully set, otherwise, return -1
 */
int GIPY_pinSetEdge(int pPin, pinEdge pEdge){
	dbgMessage((DBG_PARAMS, "[START] Try to set the edge (Pin: %d, value: %d)", pPin, pEdge));

	//Check whether the pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError((DBG_PARAMS, "Invalid pin number: %d", pPin));
		printError("Unable to set edge: invalid pin (%d)", pPin);
		return -1;
	}

	//Open the edge file
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH"/gpio%d/edge", pPin);
	int file = open(stamp, O_WRONLY);
	if(file == -1){
		dbgError((DBG_PARAMS, "Unreachable file %s", stamp));
		printError("Unable to set the edge for pin %d", pPin);
		return -1;
	}

	//Try to write new edge in opened file
	int writeError;
	switch(pEdge){
		case RISING:
			writeError = (write(file, "rising", 7) == 7) ? 1 : 0;
			break;
		case FALLING:
			writeError = (write(file, "falling", 8) == 8) ? 1 : 0;
			break;
		case BOTH:
			writeError = (write(file, "both", 5) == 5) ? 1 : 0;
			break;
		default:
			writeError = (write(file, "none", 5) == 5) ? 1 : 0;
			break;
	}

	//Check whether write process success
	if(writeError != 1){
		close(file);
		dbgError((DBG_PARAMS, "Unable to write in %s with value %d", stamp, pEdge));
		printError("Unable to set pin %d edge!", pPin);
		return -1;
	}
	close(file);
	dbgMessage((DBG_PARAMS, "[SUCCESS] Pin %d edge set", pPin));
	return 1;
}


//*****************************************************************************
// GPIO Read / Write functions
//*****************************************************************************
/**
 * @brief	Read the value from a specific pin
 *
 * @param pPin	int the pin number where to read
 * @return		Int value corresponding to the pin state (0 or 1)
 */
int GIPY_pinRead(int pPin){
	dbgMessage((DBG_PARAMS, "[START] Try to read pin %d", pPin));

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError((DBG_PARAMS, "Invalid pin number: %d", pPin));
		printError("Unable to read from pin %d! Invalid pin", pPin);
		return -1;
	}

	//Create the GPIO value path and try to open it
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH"gpio%d/value", pPin);
	int file = open(stamp, O_RDONLY);
	if(file < 1){
		dbgError((DBG_PARAMS, "Unable to open the file: %s", stamp));
		printError("Unable to read from pin %d", pPin);
		return -1;
	}
	
	//Read from the file
	char buff;
	int n;
	if((n = read(file, &buff, 1)) == -1){
		close(file);
		dbgError((DBG_PARAMS, "Unable to read from file: %s", stamp));
		printError("Unable to read from pin %d", pPin);
		return -1;
	}
	close(file);
	dbgMessage((DBG_PARAMS, "[SUCCESS] to read pin %d", pPin));
	return buff-'0'; //n equals read value
}

/**
 * @brief	Write a value for a specific GPIO Pin
 * @detail	The pin must have been enabled before. If the value given is not 
 * 			valid, the process is stopped and return the error state
 *
 * @param pPin		Pin number where to write
 * @param pValue	Value to set to this pin (should be from pinValue enum)
 * @return			1 if successfully written, otherwise, return -1
 */
int GIPY_pinWrite(int pPin, pinValue pValue){
	dbgMessage((DBG_PARAMS, "[START] Try to write in pin %d", pPin));

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError((DBG_PARAMS, "Invalid pin number: %d", pPin));
		printError("Unable to write in %d! Invalid pin!", pPin);
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
		dbgError((DBG_PARAMS, "Unable to open the file: %s", stamp));
		printError("Unable to write in pin %d", pPin);
		return -1;
	}

	//Try to write the value in the GPIO value file
	char buff[2];
	sprintf(buff, "%d", pValue);
	if(write(file, buff, 2) != 2){
		close(file);
		dbgError((DBG_PARAMS, "Unable to write the value %d in file %s", pValue, stamp));
		printError("Unable to write in pin %d", pPin);
		return -1;
	}
	close(file);
	dbgMessage((DBG_PARAMS, "[SUCCESS] Successfully written in pin %d", pPin));
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

