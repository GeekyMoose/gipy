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
// Constants - General variable
// ****************************************************************************
#define TRUE		(1==1)
#define FALSE		(1==42)

#define LED_BLUE	17
#define LED_WHITE	18
#define LED_GREEN	23
#define LED_RED		24
#define BUTTON_1	21
#define BUTTON_2	22
#define BUTTON_3	10
#define UNSET		0

#define CODE_LENGTH	5

static int code[CODE_LENGTH]; //the code
static int pos = 0; //Current read position in the code
static int exitGame = FALSE;


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
	GIPY_pinExport(LED_BLUE);
	GIPY_pinExport(LED_WHITE);
	GIPY_pinExport(LED_GREEN);
	GIPY_pinExport(LED_RED);
	GIPY_pinExport(BUTTON_1);
	GIPY_pinExport(BUTTON_2);
	GIPY_pinExport(BUTTON_3);
	
	//Set LED pins directions
	GIPY_pinSetDirectionOut(LED_BLUE);
	GIPY_pinSetDirectionOut(LED_WHITE);
	GIPY_pinSetDirectionOut(LED_GREEN);
	GIPY_pinSetDirectionOut(LED_RED);

	//Set Buttons pins directions
	GIPY_pinSetDirectionIn(BUTTON_1);
	GIPY_pinSetDirectionIn(BUTTON_2);
	GIPY_pinSetDirectionIn(BUTTON_3);

	//By default, all LED are closed
	GIPY_pinWrite(LED_BLUE, LOGIC_ZERO);
	GIPY_pinWrite(LED_WHITE, LOGIC_ZERO);
	GIPY_pinWrite(LED_GREEN, LOGIC_ZERO);
	GIPY_pinWrite(LED_RED, LOGIC_ZERO);

	//Set Edge for buttons
	GIPY_pinSetEdgeFalling(BUTTON_1);
	GIPY_pinSetEdgeFalling(BUTTON_2);
	GIPY_pinSetEdgeFalling(BUTTON_3);

	//Set interrupt handler
	GIPY_pinCreateInterrupt(BUTTON_1, &interruptButton1);
	GIPY_pinCreateInterrupt(BUTTON_2, &interruptButton2);
	GIPY_pinCreateInterrupt(BUTTON_3, &interruptButton3);
	return 1;
}

/**
 * @brief			Stop GPIO Elements
 *
 * @return int		1 if no error otherwise -1
 */
int unsetGipyElements(){
	GIPY_pinUnexport(LED_BLUE);
	GIPY_pinUnexport(LED_WHITE);
	GIPY_pinUnexport(LED_GREEN);
	GIPY_pinUnexport(LED_RED);
	GIPY_pinUnexport(BUTTON_1);
	GIPY_pinUnexport(BUTTON_2);
	GIPY_pinUnexport(BUTTON_3);
	return 1;
}


// ****************************************************************************
// Interrups handlers - Tools functions
// ****************************************************************************
void interruptButton1(){
	dbgInfo("Interrupt : Button1");
	GIPY_pinWrite(LED_BLUE, LOGIC_ZERO);
}
void interruptButton2(){
	dbgInfo("Interrupt : Button2");
	GIPY_pinWrite(LED_WHITE, LOGIC_ZERO);
}
void interruptButton3(){
	dbgInfo("Interrupt : Button3");
	GIPY_pinWrite(LED_GREEN, LOGIC_ZERO);
}

/**
 * @brief		Reset the code to unset state
 */
void resetCode(){
	pos = 0; //Reset read position to beginning
	int k=0;
	for(k=0; k<CODE_LENGTH; k++){
		code[k] = UNSET;
	}
}


// ****************************************************************************
// Game functions
// ****************************************************************************

void createCode(){
}

void game(){
}

/**
 * Main function
 */
int main(int argc, char **argv){
	setGipyElements();
	resetCode();
	while(exitGame == FALSE){
	}
	unsetGipyElements();
	return EXIT_SUCCESS;
}








