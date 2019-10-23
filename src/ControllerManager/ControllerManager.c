/*
 * ControllerManager.c
 *
 *  Created on: Oct 21, 2019
 *      Author: gabri
 */

#include "ControllerManager.h"
#include "../DisplayManager/DisplayManager.h"
#include "../drv/Keyboard/Keyboard.h"

unsigned short _lastSelectedConfig = 0;
unsigned short _selectedConfig = 0;

ControllerConfig _st_controllerConfig =
	{MOTOR_OFF, 100, 99, CURRENT_HIGH, FULL_STEP, CLOCKWISE};

SystemState _systemState = STATE_MAIN;

void Do_MainRoutine(unsigned long key);
void Do_UpdateFreqRoutine(unsigned long key);
void Do_UpdateStepsRoutine(unsigned long key);

void ControllerManager_Init(void)
{
	DisplayManager_Init(_st_controllerConfig);
	DisplayManager_FirstUpdateSelected(_selectedConfig);
}

void ControllerManager_ProccessInputs(unsigned long key)
{

	switch (_systemState) {
		case STATE_MAIN:
			Do_MainRoutine(key);
			break;
		case STATE_UPDATE_FREQ:
			Do_UpdateFreqRoutine(key);
			break;
		case STATE_UPDATE_STEPS:
			Do_UpdateStepsRoutine(key);
			break;
		default:
			break;
	}

}

void ControllerManager_UpdateOutputs(void)
{

}

unsigned long ControllerManager_GetTaskDelay(void)
{
	return (unsigned long)(2000/(double)_st_controllerConfig.frequency);
}

void Do_MainRoutine(unsigned long key)
{
	switch (key) {
		case KEY_ONE:

			_lastSelectedConfig = _selectedConfig;
			if(_selectedConfig<5) _selectedConfig++;
			else _selectedConfig = 0;
			DisplayManager_UpdateSelected(_lastSelectedConfig, _selectedConfig);

			break;
		case KEY_TWO:

			_lastSelectedConfig = _selectedConfig;
			if(_selectedConfig>0) _selectedConfig--;
			else _selectedConfig = 5;
			DisplayManager_UpdateSelected(_lastSelectedConfig, _selectedConfig);

			break;
		case KEY_THREE:

			if(_selectedConfig == 0)
			{
				_st_controllerConfig.motorState = (_st_controllerConfig.motorState==MOTOR_OFF) ? MOTOR_ON : MOTOR_OFF;
				DisplayManager_UpdateMotorState(_st_controllerConfig.motorState);

			} else if(_selectedConfig == 1)
			{
				_systemState = STATE_UPDATE_FREQ;

			} else if(_selectedConfig == 2)
			{
				_systemState = STATE_UPDATE_STEPS;

			} else if(_selectedConfig == 3)
			{
				_st_controllerConfig.activeCurrent = (_st_controllerConfig.activeCurrent==CURRENT_HIGH) ? CURRENT_LOW : CURRENT_HIGH;
				DisplayManager_UpdateActiveCurrent(_st_controllerConfig.activeCurrent);

			} else if(_selectedConfig == 4)
			{
				_st_controllerConfig.stepType = (_st_controllerConfig.stepType==FULL_STEP) ? HALF_STEP : FULL_STEP;
				DisplayManager_UpdateStepType(_st_controllerConfig.stepType);

			} else if(_selectedConfig == 5)
			{
				_st_controllerConfig.clockDirection = (_st_controllerConfig.clockDirection==CLOCKWISE) ? COUNTERCLOCK : CLOCKWISE;
				DisplayManager_UpdateClockDirection(_st_controllerConfig.clockDirection);

			}

			break;
		case KEY_FOUR:

			break;
		case KEY_FIVE:

			break;
		default:
			break;
	}

}

void Do_UpdateFreqRoutine(unsigned long key)
{

}

void Do_UpdateStepsRoutine(unsigned long key)
{

}

