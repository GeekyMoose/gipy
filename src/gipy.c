/*
 * ****************************************************************************
 * GIPY Library
 * Manage Raspberry Pi GPIO
 *
 * Since:	Nov 29, 2015
 * Author:	Constantin MASSON <constantin.grinda@gmail.com>
 * ****************************************************************************
 */

#include "gipy.h"


//*****************************************************************************
// Privat header (Static function / Vars)
//*****************************************************************************
static int isValidPinNumber(const int);
static void *pinInterruptHandler(void*);


/*
 * @brief	Match the /sys/class/gpio/gpioX/value opened file
 * @details	If this file is -1, means that pin is unexported
 * 			Note that many elt in array are useless, the goal here 
 * 			is to get the value for pin x just by using [x]
 */
static int valueFds[30] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

/*
 * @brief	ISR function for each pin
 * @details	The function for pin x is at [x]
 */
static void (*isrFunctions[30])(void);



//*****************************************************************************
// GPIO Export / Unexport functions
//*****************************************************************************
/**
 * @brief			Export (Enable) a GPIO Pin.
 *
 * @param pPin		Pin number to enable
 * @return GE_OK	If no error
 * @return GE_PIN	If pin is not valid
 * @return GE_PERM	If unable to open export (or value) file
 * @return GE_IO	If unable to read in export file
 */
pirror GIPY_pinExport(int pPin){
	dbgInfo("Try to enable pin %d", pPin);

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Open the export sys file, check if successfully opened
	int file = open(GPIO_PATH_EXPORT, O_WRONLY);
	if(file == -1){
		dbgError("Unable to open (WRONLY) export file: "GPIO_PATH_EXPORT);
		return GE_PERM;
	}

	//Write into file that this pin is set
	char tamp[3];
	sprintf(tamp, "%d", pPin);
	if(write(file, tamp, sizeof(pPin)) != sizeof(pPin)){
		dbgError("Unable to write %d in file: "GPIO_PATH_EXPORT, pPin);
		close(file);
		return GE_IO;
	}
	close(file); //This close the export file

	//Open the value file (And keep it open in valueFds array)
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH_VALUE, pPin);
	file = open(stamp, O_RDWR);
	if(file == -1){
		dbgError("Unable to open (RDWR) value file: %s", stamp);
		return GE_PERM;
	}
	valueFds[pPin] = file; //Keep in memory the value file for this pin
	dbgInfo("Pin %d enabled (fd: %d)", pPin, file);
	return GE_OK;
}

/**
 * @brief			Unexport (disable) a pin
 *
 * @param pPin		Pin to disable
 * @return GE_OK	If no error
 * @return GE_PIN	If pin is not valid
 * @return GE_PERM	If unable to open unexport file
 * @return GE_IO	If unable to write in export file
 */
pirror GIPY_pinUnexport(int pPin){
	dbgInfo("Try to disbale pin %d", pPin);

	//Check whether pin number is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Try to open unexport file
	int file = open(GPIO_PATH_UNEXPORT, O_WRONLY);
	if(file == -1){
		dbgError("Unable to read (Write) export file: "GPIO_PATH_UNEXPORT);
		return GE_NOENT;
	}

	//Write this pin in unexport file
	char tamp[3];
	sprintf(tamp, "%d", pPin);
	if(write(file, tamp, sizeof(tamp)) != sizeof(tamp)){
		dbgError("Unable to write %d in export file: "GPIO_PATH_UNEXPORT, pPin);
		close(file);
		return GE_IO;
	}
	close(file);

	//Close the value file descriptor for this pin
	close(valueFds[pPin]);
	dbgInfo("Pin %d disabled (fd: %d)", pPin, valueFds[pPin]);
	valueFds[pPin] = -1;
	return GE_OK;
}


//*****************************************************************************
// Pin set direction functions
//*****************************************************************************
/**
 * @brief				Set the pin direction to In 
 *
 * @param pPin			Pin to set
 * @return GE_OK		If no error
 * @return GE_PIN		If pin number is not valid
 * @return GE_PERM		If pin not exported
 * @return GE_NOENT		If unable to open (Write) dir file
 * @return GE_PINDIR	If the pin direction is not valid
 * @return GE_IO		If unable to write in direction file
 */
pirror GIPY_pinSetDirectionIn(int pPin){
	return GIPY_pinSetDirection(pPin, IN);
}

/**
 * @brief				Set the pin direction to Out
 *
 * @param pPin			Pin to set
 * @return GE_OK		If no error
 * @return GE_PIN		If pin number is not valid
 * @return GE_PERM		If pin not exported
 * @return GE_NOENT		If unable to open (Write) dir file
 * @return GE_PINDIR	If the pin direction is not valid
 * @return GE_IO		If unable to write in direction file
 */
pirror GIPY_pinSetDirectionOut(int pPin){
	return GIPY_pinSetDirection(pPin, OUT);
}

/**
 * @brief				Set the pin direction to low
 *
 * @param pPin			Pin to set
 * @return GE_OK		If no error
 * @return GE_PIN		If pin number is not valid
 * @return GE_PERM		If pin not exported
 * @return GE_NOENT		If unable to open (Write) dir file
 * @return GE_PINDIR	If the pin direction is not valid
 * @return GE_IO		If unable to write in direction file
 */
pirror GIPY_pinSetDirectionLow(int pPin){
	return GIPY_pinSetDirection(pPin, LOW);
}

/**
 * @brief				Set the pin direction to high
 *
 * @param pPin			Pin to set
 * @return GE_OK		If no error
 * @return GE_PIN		If pin number is not valid
 * @return GE_PERM		If pin not exported
 * @return GE_NOENT		If unable to open (Write) dir file
 * @return GE_PINDIR	If the pin direction is not valid
 * @return GE_IO		If unable to write in direction file
 */
pirror GIPY_pinSetDirectionHigh(int pPin){
	return GIPY_pinSetDirection(pPin, HIGH);
}

/**
 * @brief				Set the pin direction
 * @details				If direction is not valid, nothing is done
 *
 * @param pPin			Pin to set
 * @param pPinDir		Direction to set (From PinDirection enum)
 * @return GE_OK		If no error
 * @return GE_PIN		If pin number is not valid
 * @return GE_PERM		If pin not exported
 * @return GE_NOENT		If unable to open (Write) dir file
 * @return GE_PINDIR	If the pin direction is not valid
 * @return GE_IO		If unable to write in direction file
 */
pirror GIPY_pinSetDirection(int pPin, pinDirection pPinDir){
	dbgInfo("Try to change direction pin %d to %d", pPin, pPinDir);

	//Check whether pin number is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Pin must be enabled
	if(valueFds[pPin] == -1){
		dbgError("Try to set a dirrection to unexported pin %d", pPin);
		return GE_PERM;
	}

	//Open direction folder
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH_DIRECTION, pPin);
	int file = open(stamp, O_WRONLY);
	if(file == -1){
		dbgError("Unable to open (Write) file %s for pin %d", stamp, pPin);
		return GE_NOENT;
	}

	//Try to write the new direction in file according to dir parameter
	int writeError;
	switch(pPinDir){
		case IN:
			writeError = (write(file, "in", 2) == 2) ? 1 : -1;
			break;
		case OUT:
			writeError = (write(file, "out", 3) == 3) ? 1 : -1;
			break;
		case LOW:
			writeError = (write(file, "low", 3) == 3) ? 1 : -1;
			break;
		case HIGH:
			writeError = (write(file, "high", 4) == 4) ? 1 : -1;
			break;
		default:
			//Default mean the pin dir is not valid
			close(file);
			dbgError("Invalid pin dir");
			writeError = GE_PINDIR;
			break;
	}

	//Check the write process was successfully done
	if(writeError != 1){
		close(file);
		dbgError("Unable to write in %s with value %d", stamp, pPinDir);
		return GE_IO;
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
 * @return GE_OK	If no error
 * @return GE_PIN	If invalid pin
 * @return GE_PERM	If pin is not already exported
 * @return GE_NOENT	If edge file unreachable
 * @return GE_IO	If unable to read edge file
 */
pirror GIPY_pinSetEdgeNone(int pPin){
	return GIPY_pinSetEdge(pPin, NONE);
}

/**
 * @brief			Set edge of the given pin to Rising
 *
 * @param pPin		Pin to set
 * @return GE_OK	If no error
 * @return GE_PIN	If invalid pin
 * @return GE_PERM	If pin is not already exported
 * @return GE_NOENT	If edge file unreachable
 * @return GE_IO	If unable to read edge file
 */
pirror GIPY_pinSetEdgeRising(int pPin){
	return GIPY_pinSetEdge(pPin, RISING);
}

/**
 * @brief			Set edge of the given pin to falling
 *
 * @param pPin		Pin to set
 * @return GE_OK	If no error
 * @return GE_PIN	If invalid pin
 * @return GE_PERM	If pin is not already exported
 * @return GE_NOENT	If edge file unreachable
 * @return GE_IO	If unable to read edge file
 */
pirror GIPY_pinSetEdgeFalling(int pPin){
	return GIPY_pinSetEdge(pPin, FALLING);
}

/**
 * @brief			Set edge of the given pin to both
 *
 * @param pPin		Pin to set
 * @return GE_OK	If no error
 * @return GE_PIN	If invalid pin
 * @return GE_PERM	If pin is not already exported
 * @return GE_NOENT	If edge file unreachable
 * @return GE_IO	If unable to read edge file
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
 * @return GE_OK	If no error
 * @return GE_PIN	If invalid pin
 * @return GE_PERM	If pin is not already exported
 * @return GE_NOENT	If edge file unreachable
 * @return GE_IO	If unable to read edge file
 */
pirror GIPY_pinSetEdge(int pPin, pinEdge pEdge){
	dbgInfo("Try to set the edge (Pin: %d, value: %d)", pPin, pEdge);

	//Check whether the pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Pin must be enabled
	if(valueFds[pPin] == -1){
		dbgError("Try to set edge %d to unexported pin %d", pEdge,  pPin);
		return GE_PERM;
	}

	//Open the edge file
	char stamp[BUFSIZ];
	sprintf(stamp, GPIO_PATH_EDGE, pPin);
	int file = open(stamp, O_RDWR);
	if(file == -1){
		dbgError("Unable to open (Write) edge file %s", stamp);
		return GE_NOENT;
	}

	//Try to write new edge in opened file
	int writeError;
	//NOTE: the spaces are important to delete old content (Might be a better way)
	switch(pEdge){
		case RISING:
			writeError = (write(file, "rising", 6) == 6) ? 1 : 0;
			break;
		case FALLING:
			writeError = (write(file, "falling", 7) == 7) ? 1 : 0;
			break;
		case BOTH:
			writeError = (write(file, "both", 4) == 4) ? 1 : 0;
			break;
		default:
			writeError = (write(file, "none", 4) == 4) ? 1 : 0;
			break;
	}

	//Check whether write process success
	if(writeError != 1){
		close(file);
		dbgError("Unable to write in %s with value %d", stamp, pEdge);
		return GE_IO;
	}
	close(file);
	dbgInfo("Pin %d edge set", pPin);
	return GE_OK;
}


//*****************************************************************************
// GPIO Read / Write functions
//*****************************************************************************
/**
 * @brief			Read a pin value
 *
 * @param pPin		Pin number to read
 * @param pRead		Pointer toward read value to fill
 * @return GE_OK	If no error
 * @return GE_PIN	If pin is not valid
 * @return GE_PERM	If pin is not exported
 * @return GE_IO	If unable to read from value file
 */
pirror GIPY_pinRead(int pPin, int *pRead){
	dbgInfo("Try to read pin %d (fd: %d)", pPin, valueFds[pPin]);

	//Check if pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Pin must be enabled
	if(valueFds[pPin] == -1){
		dbgError("Try to read from unexported pin %d",  pPin);
		return GE_PERM;
	}

	//Read from the file
	char buff;
	lseek(valueFds[pPin], 0, SEEK_SET); //Go back beginning file
	if(read(valueFds[pPin], &buff, 1) == -1){
		dbgError("Unable to read from value file for pin: %d", pPin);
		return GE_IO;
	}
	*pRead = buff-'0'; //n equals read value
	dbgInfo("Pin %d read, value: %d", pPin, *pRead);
	return GE_OK;
}

/**
 * @brief				Write a value for a specific GPIO Pin
 * @detail				The pin must have been enabled before. 
 * 						Nothing done is invalid value given
 *
 * @param pPin			Pin number where to write
 * @param pValue		Value to set to this pin (should be from pinValue enum)
 * @return GE_OK		If no error
 * @return GE_PIN		If pin number is not valid
 * @return GE_PINVAL	If pin value is not valid
 * @return GE_IO		If unable to read from value file
 */
pirror GIPY_pinWrite(int pPin, pinValue pValue){
	dbgInfo("Try to write %d in pin %d (fd: %d)", pValue, pPin, valueFds[pPin]);

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

	//Pin must be enabled
	if(valueFds[pPin] == -1){
		dbgError("Try to read from unexported pin %d",  pPin);
		return GE_PERM;
	}

	//try to write the value in the gpio value file
	char buff = (char) (pValue+'0');
	lseek(valueFds[pPin], 0, SEEK_SET); //Go back beginning file
	if(write(valueFds[pPin], &buff, 1) != 1){
		dbgError("Unable to write in value file for pin: %d", pPin);
		return GE_IO;
	}
	dbgInfo("Successfully written %d in pin %d", pValue, pPin);
	return GE_OK;
}


//*****************************************************************************
// Interrupt functions
//*****************************************************************************
/**
 * @brief				Create an interrupt for specific pin
 * @details				At most one interrupt can be created for a pin
 * 						Attention: if this pin already got an interrupt set, 
 * 						it will be lost and replaced by this new one.
 *
 * @param				pin linked with interrupt
 * @param				function to execut if interrupt generated
 * @return GE_OK		If no error
 * @return GE_PIN		If invalid pin number
 * @return GE_PERM		If pin not exported
 */
pirror GIPY_pinCreateInterrupt(int pPin, void (*function)(void)){
	dbgInfo("Try to create interrupt for pin %d", pPin);
	
	//Check wither pin is valid
	if(isValidPinNumber(pPin)==FALSE){
		dbgError("Invalid pin number: %d", pPin);
		return GE_PIN;
	}

	//Pin must be enabled
	if(valueFds[pPin] == -1){
		dbgError("Try to set interrupt to unexported pin %d", pPin);
		return GE_PERM;
	}

	//Create a thread which check for event. The function isr is saved
	pthread_t threadId;
	isrFunctions[pPin] = function; //Change handler function
	pthread_create(&threadId, NULL, &pinInterruptHandler, (void *)(intptr_t)pPin);
	return GE_OK;
}

/**
 * @brief			create the pin interrupt process for a pin
 * @details			Private function. Called by the public create interrupt 
 * 					function. This function is executed inside a thread. 
 * 					If interrupt generated, the function isrFunctions is 
 * 					executed for given pin.
 *
 * @param			pin to link with an interrupt process
 * @return void
 */
static void *pinInterruptHandler(void *pPin){
	int intPin = (intptr_t)pPin;
	char buff[2];
	dbgInfo("Start pinInterruptHandler for pin %d", intPin);
	struct pollfd		pollstruct;
	pollstruct.fd		= valueFds[intPin];
	pollstruct.events	= POLLPRI;

	//Loop blocked by poll. Wait for any event and call function if interrupt
	for(;;){
		dbgInfo("* Wait for event (pin: %d, df: %d)", intPin, pollstruct.fd);
		//If the pin has been unexported since the interrupt creation
		if(pollstruct.fd == -1){
			dbgInfo("Attention: pin %d unexported will interrupt running", intPin);
			break; //Stop interrupt handling
		}

		//If an event is detected, call isr function
		if(poll(&pollstruct, 1, -1) > 0){
			if(pollstruct.revents & POLLPRI){
				//Dummy read to clear the interrupt
				read(pollstruct.fd, buff, 2);
				lseek(pollstruct.fd, 0, SEEK_SET);
				dbgInfo("Poll pin %d, df %d", intPin, pollstruct.fd);
				isrFunctions[intPin]();
			}
		}
	}
	dbgInfo("Error pinInterrupHandler for pin %d: end of function reached", intPin);
	return NULL;
}


//*****************************************************************************
// Tools functions
//*****************************************************************************
/**
 * @brief	Check whether the pin number is valid
 *
 * @param	int the pin number
 * @return	TRUE if valid, otherwise, return FALSE
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

