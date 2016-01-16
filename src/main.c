/*
 * LINE
 * C project for Buildroot
 * Manage GPIO LED and Buttons
 *
 * Date : Nov 19, 2015
 * Author : Constantin MASSON
 *
 * Temporary debug file
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gipy.h"

// Prototypes
void interruptHandlerTest();

/**
 * Main function
 */
int main(int argc, char **argv){
	printf("\n\n***** Start *****\n");

	int			pin		= 18;
	int			readValue;

	GIPY_pinExport(pin);
	printf(" * Value default: %d\n", readValue);

	GIPY_pinWrite(pin, LOGIC_ZERO);
	GIPY_pinRead(pin, &readValue);
	printf(" * Value after write LOGIC_ZERO: %d\n", readValue);

	GIPY_pinWrite(pin, LOGIC_ONE);
	GIPY_pinRead(pin, &readValue);
	printf(" * Value after write LOGIC_ONE: %d\n", readValue);

	GIPY_pinWrite(pin, LOGIC_ZERO);
	GIPY_pinRead(pin, &readValue);
	printf(" * Value after write LOGIC_ZERO: %d\n", readValue);

	//Direction test
	GIPY_pinSetDirection(pin, LOW);
	GIPY_pinSetDirectionIn(pin);
	GIPY_pinSetDirectionOut(pin);
	GIPY_pinSetDirectionLow(pin);
	GIPY_pinSetDirectionHigh(pin);
	
	//Edge test
	GIPY_pinSetEdgeNone(pin);
	GIPY_pinSetEdgeRising(pin);
	GIPY_pinSetEdgeFalling(pin);
	GIPY_pinSetEdgeBoth(pin);

	//Test interrupt
	GIPY_pinCreateInterrupt(pin, interruptHandlerTest);

	sleep(1);
	GIPY_pinWrite(pin, LOGIC_ZERO);
	GIPY_pinWrite(pin, LOGIC_ZERO);

	while(1){
	}

	GIPY_pinUnexport(pin);

	printf("\n***** End *****\n\n");
	return EXIT_SUCCESS;
}


void interruptHandlerTest(){
	printf("InterruptHandlerTest - Called\n");
}











