/*
 * ****************************************************************************
 * GIPY Library
 *
 * Date : Jan 16, 2015
 * Author : Constantin MASSON
 *
 * This is an example proram. The goal is to find the secret code.
 * One player create code using 3 buttons. The other player must find the code
 * ****************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gipy.h"

//Function prototypes
void interruptButton1();
void interruptButton2();
void interruptButton3();


// ****************************************************************************
// Init functions
// ****************************************************************************
/**
 * @brief			Set all gpio components
 * @detail			4 LED and 3 Buttons are used
 * 					LED are set to GPIO 17(B), 18(W), 23(G), 24(R)
 * 					Buttons are set to GPIO 21, 22, 10
 * 					Buttons create interrupt if holded
 *
 * @return int		1 if successfully set, otherwise, return -1
 */
int setGipyElements(){
	//Enable all pins
	GIPY_pinExport(17);
	GIPY_pinExport(18);
	GIPY_pinExport(23);
	GIPY_pinExport(24);
	GIPY_pinExport(21);
	GIPY_pinExport(22);
	GIPY_pinExport(10);
	
	//Set LED pins directions
	GIPY_pinSetDirectionOut(17);
	GIPY_pinSetDirectionOut(18);
	GIPY_pinSetDirectionOut(23);
	GIPY_pinSetDirectionOut(24);

	//Set Buttons pins directions
	GIPY_pinSetDirectionIn(21);
	GIPY_pinSetDirectionIn(22);
	GIPY_pinSetDirectionIn(10);

	//Set Edge for buttons
	GIPY_pinSetEdgeFalling(21);
	GIPY_pinSetEdgeFalling(22);
	GIPY_pinSetEdgeFalling(10);

	//Set interrupt handler
	GIPY_pinCreateInterrupt(21, &interruptButton1);
	GIPY_pinCreateInterrupt(22, &interruptButton2);
	GIPY_pinCreateInterrupt(10, &interruptButton3);
	return 1;
}

/**
 * @brief			Stop GPIO Elements
 *
 * @return int		1 if no error otherwise -1
 */
int unsetGipyElements(){
	GIPY_pinUnexport(17);
	GIPY_pinUnexport(18);
	GIPY_pinUnexport(23);
	GIPY_pinUnexport(24);
	GIPY_pinUnexport(21);
	GIPY_pinUnexport(22);
	GIPY_pinUnexport(10);
	return 1;
}


// ****************************************************************************
// Game functions
// ****************************************************************************
void interruptButton1(){
	dbgInfo("Interrupt : Button1");
}
void interruptButton2(){
	dbgInfo("Interrupt : Button2");
}
void interruptButton3(){
	dbgInfo("Interrupt : Button3");
}



/**
 * Main function
 */
int main(int argc, char **argv){
	setGipyElements();
	unsetGipyElements();
	return EXIT_SUCCESS;
}








