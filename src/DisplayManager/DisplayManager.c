/*
 * DisplayManager.c
 *
 *  Created on: Oct 21, 2019
 *      Author: gabri
 */

#include "DisplayManager.h"
#include "ScreenLayout.h"
#include "../ControllerManager/ControllerManager.h"
#include "../drv/LCD/Nokia5110.h"

st_displayLines st_defaultScreen;
st_displayLines st_configFreqScreen;
st_displayLines st_configStepsScreen;

st_displayLines initDefaultScreen(ControllerConfig st_controllerConfig);
st_displayLines initConfigFreqScreen(ControllerConfig st_controllerConfig);
st_displayLines initConfigStepsScreen(ControllerConfig st_controllerConfig);

void updateFullDisplay(st_displayLines screen)
{
	Nokia5110_OutString(screen.firstLine);
	Nokia5110_OutString(screen.secondLine);
	Nokia5110_OutString(screen.thirdLine);
	Nokia5110_OutString(screen.fourthLine);
	Nokia5110_OutString(screen.fivethLine);
	Nokia5110_OutString(screen.sixthLine);
}

void DisplayManager_Init(ControllerConfig st_controllerConfig)
{
	Nokia5110_Init();
	Nokia5110_Clear();

	st_defaultScreen     = initDefaultScreen(st_controllerConfig);
	st_configFreqScreen  = initConfigFreqScreen(st_controllerConfig);
	st_configStepsScreen = initConfigStepsScreen(st_controllerConfig);

	updateFullDisplay(st_defaultScreen);
}

void DisplayManager_ChangeScreen(SystemState state, ControllerConfig st_config)
{
	Nokia5110_Clear();

}

void DisplayManager_FirstUpdateSelected(unsigned short newIdx)
{
	Nokia5110_SetCursor(0, newIdx);
	Nokia5110_OutChar('>');
}

void DisplayManager_UpdateSelected(unsigned short lastIdx, unsigned short newIdx)
{
	Nokia5110_SetCursor(0, lastIdx);
	Nokia5110_OutChar(' ');
	Nokia5110_SetCursor(0, newIdx);
	Nokia5110_OutChar('>');
}

void DisplayManager_UpdateMotorState(MotorState newState)
{
	Nokia5110_SetCursor(7, 0);
	(newState==MOTOR_OFF) ? Nokia5110_OutString("OFF") : Nokia5110_OutString("ON ");
}

void DisplayManager_UpdateActiveCurrent(ActiveCurrent newActiveCurr)
{
	Nokia5110_SetCursor(7, 3);
	(newActiveCurr==CURRENT_HIGH) ? Nokia5110_OutString("High") : Nokia5110_OutString("Low ");
}

void DisplayManager_UpdateStepType(StepType newStepType)
{
	Nokia5110_SetCursor(1, 4);
	(newStepType==FULL_STEP) ? Nokia5110_OutString("Full") : Nokia5110_OutString("Half");
}

void DisplayManager_UpdateClockDirection(ClockDirection newClockDir)
{
	Nokia5110_SetCursor(1, 5);
	(newClockDir==CLOCKWISE) ? Nokia5110_OutString("Clockwise  ") : Nokia5110_OutString("Counterclck");
}

st_displayLines initDefaultScreen(ControllerConfig st_controllerConfig)
{
	st_displayLines displayLines = {
		" Motor OFF  ",
		" Freq.   0Hz",
		" Steps     0",
		" Curr. High ",
		" Full Step  ",
		" Clockwise  "};
	return displayLines;
}
st_displayLines initConfigFreqScreen(ControllerConfig st_controllerConfig)
{
	st_displayLines displayLines = {
		" Motor OFF  ",
		" Freq.   0Hz",
		" Steps     0",
		" Curr. High ",
		" Full Step  ",
		" Clockwise  "};
	return displayLines;
}
st_displayLines initConfigStepsScreen(ControllerConfig st_controllerConfig)
{
	st_displayLines displayLines = {
		" Motor OFF  ",
		" Freq.   0Hz",
		" Steps     0",
		" Curr. High ",
		" Full Step  ",
		" Clockwise  "};
	return displayLines;
}
