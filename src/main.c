/*
 * LINE
 * C project for Buildroot
 * Manage GPIO LED and Buttons
 *
 * Date : Nov 19, 2015
 * Author : Constantin MASSON
 */

#define DEBUG_MODE //Enable debug mode

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gipy.c"


/**
 * Main function
 */
int main(int argc, char **argv){
	printf("\n\n***** Start *****\n");
	
	int k;
	for(k=0; k<argc; k++){
		printf("argv[%d] = %s\n", k, argv[k]);
	}

	int			pin		= 18;
	pinValue	val		= LOGIC_ONE;
	pinValue	val2;

	GIPY_pinEnable(pin);
	GIPY_pinWrite(pin, val);
	val2 = GIPY_pinRead(pin);

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

	printf("Value : %d\n", val2);

	printf("\n***** End *****\n\n");
	return EXIT_SUCCESS;
}













