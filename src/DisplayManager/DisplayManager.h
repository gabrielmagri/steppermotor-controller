/*
 * DisplayManager.h
 *
 *  Created on: Oct 21, 2019
 *      Author: gabri
 */

#ifndef DISPLAYMANAGER_DISPLAYMANAGER_H_
#define DISPLAYMANAGER_DISPLAYMANAGER_H_

#include "../ControllerManager/ControllerManager.h"

void DisplayManager_Init(ControllerConfig st_controllerConfig);
void DisplayManager_ChangeScreen(SystemState state, ControllerConfig st_config);
void DisplayManager_FirstUpdateSelected(unsigned short newIdx);
void DisplayManager_UpdateSelected(unsigned short lastIdx, unsigned short newIdx);
void DisplayManager_UpdateMotorState(MotorState newState);
void DisplayManager_UpdateActiveCurrent(ActiveCurrent newActiveCurr);
void DisplayManager_UpdateStepType(StepType newStepType);
void DisplayManager_UpdateClockDirection(ClockDirection newClockDir);

#endif /* DISPLAYMANAGER_DISPLAYMANAGER_H_ */
