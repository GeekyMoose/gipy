/*
 * ****************************************************************************
 * GIPY Library
 *
 * Date : Jan 16, 2015
 * Author : Constantin MASSON
 *
 * This is an example proram. This is a terrible normal tictacboom
 * Each player touch one button. One of them will explode (Yes.. That's awful isn't it)
 * Button 2 has more chance to explode (But we could assume the one who 
 * dare pushing it down might be gratefully rewarded if he doesn't explode, for sure)
 *
 * If the boomb does not explode, red led is displayed, otherwise, all led blink
 * ****************************************************************************
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //For random tools
#include "gipy.h"

//Function prototypes
void interruptButton1();
void interruptButton2();
void playOneTurn(int);


// ****************************************************************************
// Constants - General variable
// ****************************************************************************
#define TRUE		(1==1)
#define FALSE		(1==42)

#define LED_BLUE	17
#define LED_WHITE	18
#define LED_GREEN	23
#define LED_RED		24
#define BUTTON_1	10
#define BUTTON_2	22

#define RATE_BTN_1	5  //Mean button 1 has 20% chance to explode
#define RATE_BTN_2	9 //40% chance to explode

#define TURN_DELAY	2 //Define the delay between to turn (When we push down btn)

static int 			exitGame			= FALSE;
static volatile int intInProgress		= FALSE; //Avoid button spamming


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
	
	//Set LED pins directions
	GIPY_pinSetDirectionLow(LED_BLUE);
	GIPY_pinSetDirectionLow(LED_WHITE);
	GIPY_pinSetDirectionLow(LED_GREEN);
	GIPY_pinSetDirectionLow(LED_RED);

	//Set Buttons pins directions
	GIPY_pinSetDirectionIn(BUTTON_1);
	GIPY_pinSetDirectionIn(BUTTON_2);

	//Set Edge for buttons
	GIPY_pinSetEdgeFalling(BUTTON_1);
	GIPY_pinSetEdgeFalling(BUTTON_2);

	//Set interrupt handler
	GIPY_pinCreateInterrupt(BUTTON_1, &interruptButton1);
	GIPY_pinCreateInterrupt(BUTTON_2, &interruptButton2);
	return 1;
}

/**
 * @brief			Stop GPIO Elements
 * 					Unexport all pins
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
	return 1;
}


// ****************************************************************************
// Interrups handlers - Tools functions
// ****************************************************************************
void interruptButton1(){
	//Just do nothing if interrupt already in progress (We can't spam buttons)
	if(intInProgress==TRUE){ return; }
	intInProgress=TRUE; //Here, mean no int was done
	dbgInfo("Interrupt : Button1");
	playOneTurn(RATE_BTN_1);
	intInProgress=FALSE; //Finish handling
}
void interruptButton2(){
	//Just do nothing if interrupt already in progress (We can't spam buttons)
	if(intInProgress==TRUE){ return; }
	intInProgress=TRUE; //Here, mean no int was done
	dbgInfo("Interrupt : Button2");
	playOneTurn(RATE_BTN_2);
	intInProgress=FALSE; //Finish handling
}

/**
 * @brief			Blink all exported LED
 * 					The nb of blink and time between 2 blink is given as param. 
 * 					If invalid param, default (1) will be taken
 *
 * @param pNbLoop	Nb time the LED will blink (Min 1)
 * @param pDelay	Nb sec between to blink (Min 1)
 */
void blinkAll(int pNbLoop, int pDelay){
	pNbLoop	= (pNbLoop < 1) ? 1:pNbLoop;
	pDelay	= (pDelay < 1) ? 1:pDelay;

	int k;
	for(k=0; k<pNbLoop; k++){
		//Start blink
		GIPY_pinWrite(LED_BLUE, LOGIC_ONE);
		GIPY_pinWrite(LED_WHITE, LOGIC_ONE);
		GIPY_pinWrite(LED_GREEN, LOGIC_ONE);
		GIPY_pinWrite(LED_RED, LOGIC_ONE);
		sleep(pDelay);
		GIPY_pinWrite(LED_BLUE, LOGIC_ONE);
		GIPY_pinWrite(LED_WHITE, LOGIC_ONE);
		GIPY_pinWrite(LED_GREEN, LOGIC_ONE);
		GIPY_pinWrite(LED_RED, LOGIC_ONE);
		sleep(pDelay);
	}
}


// ****************************************************************************
// Game functions
// ****************************************************************************
/**
 * @brief		Play one turn, the bomb has a given chance to explode (%)
 *
 * @param		% chance to explode
 * @return 		void
 */
void playOneTurn(int pBoomRate){
	int randNumber = (rand() % 100) + 1; //Get nb between 0-99 and add 1
	dbgInfo("Rand : %d on %d", randNumber, pBoomRate);
	//If explode
	if(randNumber <= pBoomRate){
		dbgInfo("Looser reached with rand : %d on %d", randNumber, pBoomRate);
		blinkAll(2,1);
		exitGame = TRUE; //Game is over ugly rabbit!
		return;
	}
	//Display if no boom
	GIPY_pinWrite(LED_GREEN, LOGIC_ONE);
	sleep(TURN_DELAY);
	GIPY_pinWrite(LED_GREEN, LOGIC_ZERO);
}


// ****************************************************************************
// Main function
// ****************************************************************************
/**
 * Main function
 */
int main(int argc, char **argv){
	dbgInfo("\n***** TicTacBoom is starting *****");
	setGipyElements();
	srand(time(NULL)); //seed
	while(exitGame == FALSE){
		//Nothing to do, all from interrupt
	}
	unsetGipyElements();
	dbgInfo("***** TicTacBoom is over *****\n");
	return EXIT_SUCCESS;
}









