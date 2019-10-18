#include "Keyboard.h"
#include "stm32f0xx_conf.h"

st_velocity velocity4 = { 0,   0,  0 };
st_velocity velocity3 = { 1,   15, 0, &velocity4 };
st_velocity velocity2 = { 4,   15, 0, &velocity3 };
st_velocity velocity1 = { 15,  1,  0, &velocity2 };
st_velocity velocity0 = { 0,   1,  0, &velocity1 };
st_velocity _currentVelocity;

// **************Keyboard_Init*********************
// Initialize keyboard key inputs
// Input: none
// Output: none
void Keyboard_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOC Periph clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	/* Configure PC4 as input */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	
	_currentVelocity = velocity0;
	
}

// **************Keyboard_InNoDebounce*********************
// Input from keyboard's key inputs
// This function output without debounce
// Input: none
// Output: 0 to 1F depending on keys combination
unsigned long Keyboard_InNoDebounce(void) {
    // Read and return the current keys input status
	return !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);
}

// **************Keyboard_In*********************
// Input from keyboard's key inputs
// This function output just one event per key pressed
//  being necessary to release a key in order to press it again
// Input: none 
// Output: 0 to 15 depending on keys combination
unsigned long Keyboard_In(void) {
	static unsigned long _debounceCounter            = 0;
	static unsigned long _lastCurrentPressingStatus  = 0;
	static char _outputed                            = 0;
	unsigned long _currentStatus                     = 0;
	
	// Read the current keys input status
	_currentStatus = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);
	
	// if there is some key pressed and the value still the same increment the debounce counter.
	// This statement make it only possible to activate alternating the keys.
	if( (_currentStatus == _lastCurrentPressingStatus) && (_outputed == 0) ) {
		if(_debounceCounter < DEBOUNCE_BOUND) { //Protection against overflow
			_debounceCounter++;
		}
	// On the first key release after the click event was outputed we allow to ouput something again
	} else if( (_currentStatus != _lastCurrentPressingStatus) && (_outputed == 1) ) {
		_outputed = 0;
	}
	
	// After compering current with the last value we make the last to remember the current
	_lastCurrentPressingStatus = _currentStatus;
	
	// Check for the debounce value, if reached return the key pressed
	if(_debounceCounter > DEBOUNCE_VALUE  ) {
		_outputed = 1; //Signal that the key pressed was outputed to wait for key release
		_debounceCounter = 0;
		return _currentStatus;
	} else {
		return 0x00;
	}
}

// **************Keyboard_Continuous_In*********************
// Input from keyboard's key inputs
// This function output key pressed events in a progressive
//  way, where you can hold the button and the key event will
//  start to happen each time faster.
// Input: none 
// Output: 0 to 15 depending on keys combination
unsigned long Keyboard_Continuous_In(void) {
	// waitingTimes is basically a counter of _debounceCounter overflows
	static unsigned long _waitingTimes              = 0;
	// The counter used to reach the rebounce value for key input
	static unsigned long _debounceCounter           = 0;
	// The keys pressed values from the previous cycle
	static unsigned long _lastCurrentPressingStatus = 0;
	// The variable where we store the current read for keys to be compared
	unsigned long _currentStatus                    = 0;
	// A flag that indicate that we have outputed a key pressed event
	static char _outputed                           = 0;
	
	// Read the current keys input status
	_currentStatus = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);
	
	// if there is some key pressed and the value still the same increment the debounce counter.
	if( (_currentStatus != 0) && (_currentStatus == _lastCurrentPressingStatus) ) {
		if(_debounceCounter < DEBOUNCE_BOUND) { //Protection against overflow
			_debounceCounter++;
		}
		
	// On the first key release after the click event was outputed we allow to output something again
	} else if( (_currentStatus != _lastCurrentPressingStatus) && (_outputed == 1) ) {
		_outputed = 0;
	  _debounceCounter = 0;
		_currentVelocity = velocity0;
		
	}
	
	// After comparing current with the last value we make the last to remember the current
	_lastCurrentPressingStatus = _currentStatus;
	
	// Check for the debounce value, if reached return the key pressed
	if(_debounceCounter > DEBOUNCE_VALUE  ) {
		if(_waitingTimes >= _currentVelocity.timeToWait) {
			
			// Set next velocity safely
			_currentVelocity.amountOfExecution++;
			if(_currentVelocity.next) {
				if(_currentVelocity.amountOfExecution >= _currentVelocity.timesToExecute) {
					_currentVelocity = *_currentVelocity.next;
				}
			}
			
			_outputed = 1; //Signal that the key pressed was outputed to wait for key release
			_waitingTimes = 0;
			_debounceCounter = 0;
			return _currentStatus;
		}
		_waitingTimes++;
		_debounceCounter = 0;
	}
	
	return 0x00; //default return value when there is no press event
	
}
