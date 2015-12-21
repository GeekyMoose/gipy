/*
 * LINE
 * C project for Buildroot
 * Manage GPIO LED and Buttons
 *
 * Date : Nov 19, 2015
 * Author : Constantin MASSON
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gipy.h"


/**
 * Main function
 */
int main(int argc, char **argv){
	printf("\n\n***** Start *****\n");

	int			pin		= 18;
	int			readValue;

	GIPY_pinEnable(pin);
	GIPY_pinWrite(pin, LOGIC_ZERO);
	GIPY_pinRead(pin, &readValue);

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

	printf("Value : %d\n", readValue);

	printf("\n***** End *****\n\n");
	return EXIT_SUCCESS;
}













