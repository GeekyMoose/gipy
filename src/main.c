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

	if(argc > 1){
		int		pin		= argv[1];
		pinVal	val		= LOGICAL_ZERO;
		GIPY_pinEnable(pin);
		if(argc > 2){
			pinVal = argv[2];
		}
		GIPY_pinWrite(pin, pinVal);
	}

	printf("\n***** End *****\n\n");
	return EXIT_SUCCESS;
}













