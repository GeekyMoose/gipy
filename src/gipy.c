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

//Private function
static int isValidPinNumber(const int);


//*****************************************************************************
// GPIO Export / Unexport functions
//*****************************************************************************
/**
 * @brief			Enable a GPIO Pin.
 *
 * @param pPin		int the pin int number value
 * @return			pirror value. (GE_PIN / GE_OPIMP / GE_OK)
 */
pirror GIPY_pinEnable(int pPin){
	dbgInfo("Try to enable pin %d", pPin);

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Open the export sys file, check if successfully opened
	int file = open(GPIO_PATH_EXPORT, O_WRONLY);
	if(file == -1){
		dbgError("Unreachable export file: "GPIO_PATH_EXPORT);
		return GE_OPIMP;
	}

	//Write into file that this pin is set
	char tamp[3];
	sprintf(tamp, "%d", pPin);
	if(write(file, tamp, 3) != 3){
		dbgError("Unable to write %d in file: "GPIO_PATH_EXPORT, pPin);
		close(file);
		return GE_OPIMP;
	}
	close(file);
	dbgInfo("Pin %d enabled", pPin);
	return GE_OK;
}

/**
 * @brief			Disable a pin
 *
 * @param pPin		Pin to disable
 * @return			pirror value. (GE_PIN / GE_OPIMP / GE_OK)
 */
pirror GIPY_pinDisable(int pPin){
	dbgInfo("Try to disbale pin %d", pPin);

	//Check whether pin number is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Try to open unexport file
	int file = open(GPIO_PATH_UNEXPORT, O_WRONLY);
	if(file == -1){
		dbgError("Unreachable export file: "GPIO_PATH_UNEXPORT);
		return GE_OPIMP;
	}

	//Write into file that this pin is set
	char tamp[3];
	sprintf(tamp, "%d", pPin);
	if(write(file, tamp, 3) != 3){
		dbgError("Unable to write %d in "GPIO_PATH_UNEXPORT, pPin);
		close(file);
		return GE_OPIMP;
	}
	close(file);
	dbgInfo("Pin %d disabled", pPin);
	return GE_OK;
}


//*****************************************************************************
// Pin set direction functions
//*****************************************************************************
/**
 * @brief			Set the pin direction to In 
 *
 * @param pPin		Pin to set
 * @return			pirror status (GE_OK / GE_PIN / GE_OPIMP / GE_PINDIR)
 */
pirror GIPY_pinSetDirectionIn(int pPin){
	return GIPY_pinSetDirection(pPin, IN);
}

/**
 * @brief			Set the pin direction to Out
 *
 * @param pPin		Pin to set
 * @return			pirror status (GE_OK / GE_PIN / GE_OPIMP / GE_PINDIR)
 */
pirror GIPY_pinSetDirectionOut(int pPin){
	return GIPY_pinSetDirection(pPin, OUT);
}

/**
 * @brief			Set the pin direction to low
 *
 * @param pPin		Pin to set
 * @return			pirror status (GE_OK / GE_PIN / GE_OPIMP / GE_PINDIR)
 */
pirror GIPY_pinSetDirectionLow(int pPin){
	return GIPY_pinSetDirection(pPin, LOW);
}

/**
 * @brief			Set the pin direction to high
 *
 * @param pPin		Pin to set
 * @return			pirror status (GE_OK / GE_PIN / GE_OPIMP / GE_PINDIR)
 */
pirror GIPY_pinSetDirectionHigh(int pPin){
	return GIPY_pinSetDirection(pPin, HIGH);
}

/**
 * @brief			Set the pin direction
 * @details			If direction is not valid, nothing is done
 *
 * @param pPin		Pin to set
 * @param pPinDir	Direction to set (From PinDirection enum)
 * @return			pirror status (GE_OK / GE_PIN / GE_OPIMP / GE_PINDIR)
 */
pirror GIPY_pinSetDirection(int pPin, pinDirection pPinDir){
	dbgInfo("Try to change direction pin %d to %d", pPin, pPinDir);

	//Check whether pin number is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Open direction folder
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH_DIRECTION, pPin);
	int file = open(stamp, O_WRONLY);
	if(file == -1){
		dbgError("Unreachable file %s for pin %d", stamp, pPin);
		return GE_OPIMP;
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
			close(file);
			dbgError("Invalid pin dir");
			writeError = GE_PINDIR;
			break;
	}

	//Check the write process was successfully done
	if(writeError != 1){
		close(file);
		dbgError("Unable to write in %s with value %d", stamp, pPinDir);
		return GE_OPIMP;
	}
	close(file);
	dbgInfo("Direction pin %d is now %d", pPin, pPinDir);
	return GE_OK;
}


// ****************************************************************************
// GPIO set edge functions
// ****************************************************************************

/**
 * @brief			Set edge of the given pin to None
 *
 * @param pPin		Pin to set
 * @return			pirror (GE_OK / GE_PIN / GE_OPIMP)
 */
pirror GIPY_pinSetEdgeNone(int pPin){
	return GIPY_pinSetEdge(pPin, NONE);
}

/**
 * @brief			Set edge of the given pin to Rising
 *
 * @param pPin		Pin to set
 * @return			pirror (GE_OK / GE_PIN / GE_OPIMP)
 */
pirror GIPY_pinSetEdgeRising(int pPin){
	return GIPY_pinSetEdge(pPin, RISING);
}

/**
 * @brief			Set edge of the given pin to falling
 *
 * @param pPin		Pin to set
 * @return			pirror (GE_OK / GE_PIN / GE_OPIMP)
 */
pirror GIPY_pinSetEdgeFalling(int pPin){
	return GIPY_pinSetEdge(pPin, FALLING);
}

/**
 * @brief			Set edge of the given pin to both
 *
 * @param pPin		Pin to set
 * @return			pirror (GE_OK / GE_PIN / GE_OPIMP)
 */
pirror GIPY_pinSetEdgeBoth(int pPin){
	return GIPY_pinSetEdge(pPin, BOTH);
}

/**
 * @brief			Set the edge for a given pin
 * @detail			If the given edge is not valid, edge 'none' will be set
 *
 * @param pPin		Pin to set
 * @param pEdge		Edge to set (From pinEdge enum)
 * @return			pirror (GE_OK / GE_PIN / GE_OPIMP)
 */
pirror GIPY_pinSetEdge(int pPin, pinEdge pEdge){
	dbgInfo("Try to set the edge (Pin: %d, value: %d)", pPin, pEdge);

	//Check whether the pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Open the edge file
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH_EDGE, pPin);
	int file = open(stamp, O_WRONLY);
	if(file == -1){
		dbgError("Unreachable edge file %s", stamp);
		return GE_OPIMP;
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
		dbgError("Unable to write in %s with value %d", stamp, pEdge);
		return GE_OPIMP;
	}
	close(file);
	dbgInfo("Pin %d edge set", pPin);
	return GE_OK;
}


//*****************************************************************************
// GPIO Read / Write functions
//*****************************************************************************
/**
 * @brief		Read the value from a specific pin
 *
 * @param pPin	int the pin number where to read
 * @param pRead	Read value to fill (Pointer)
 * @return		pirror (GE_OK, GE_PIN, GE_OPIMP)
 */
pirror GIPY_pinRead(int pPin, int *pRead){
	dbgInfo("Try to read pin %d", pPin);

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Create the GPIO value path and try to open it
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH_VALUE, pPin);
	int file = open(stamp, O_RDONLY);
	if(file == -1){
		dbgError("Unable to open the file: %s", stamp);
		return GE_OPIMP;
	}
	
	//Read from the file
	char buff;
	int n;
	if((n = read(file, &buff, 1)) == -1){
		close(file);
		dbgError("Unable to read from file: %s", stamp);
		return GE_OPIMP;
	}
	close(file);
	*pRead = buff-'0'; //n equals read value
	dbgInfo("Pin %d read, value: %d", pPin, *pRead);
	return GE_OK;
}

/**
 * @brief	Write a value for a specific GPIO Pin
 * @detail	The pin must have been enabled before. If the value given is not 
 * 			valid, the process is stopped and return the error state
 *
 * @param pPin		Pin number where to write
 * @param pValue	Value to set to this pin (should be from pinValue enum)
 * @param			pirror (GE_OK / GE_PIN / GE_PARAM)
 */
pirror GIPY_pinWrite(int pPin, pinValue pValue){
	dbgInfo("Try to write in pin %d", pPin);

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//check whether the pValue is valid
	if(pValue != LOGIC_ZERO && pValue != LOGIC_ONE){
		dbgError("Invalid value (%d) for pin: %d", pValue, pPin);
		return GE_PINVAL;
	}

	//Create the GPIO value path and try to open it
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH_VALUE, pPin);
	int file = open(stamp, O_WRONLY);
	if(file == -1){
		dbgError("Unable to open the file: %s", stamp);
		return GE_OPIMP;
	}

	//Try to write the value in the GPIO value file
	char buff[2];
	sprintf(buff, "%d", pValue);
	if(write(file, buff, 2) != 2){
		close(file);
		dbgError("Unable to write the value %d in file %s", pValue, stamp);
		return GE_OPIMP;
	}
	close(file);
	dbgInfo("Successfully written in pin %d", pPin);
	return GE_OK;
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
static int isValidPinNumber(const int pPin){
	static int validPins[NB_PINS] = {PINS_AVAILABLE}; //Init during first call
	int k = 0;
	for(k=0; k<NB_PINS; k++){
		if(validPins[k] == pPin){
			return TRUE;
		}
	}
	return FALSE;
}

