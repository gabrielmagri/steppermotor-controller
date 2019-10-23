/*
 * ControllerManager.h
 *
 *  Created on: Oct 21, 2019
 *      Author: gabri
 */

#ifndef CONTROLLERMANAGER_CONTROLLERMANAGER_H_
#define CONTROLLERMANAGER_CONTROLLERMANAGER_H_

typedef enum {STATE_MAIN, STATE_UPDATE_FREQ, STATE_UPDATE_STEPS} SystemState;

typedef enum {MOTOR_OFF, MOTOR_ON} MotorState;
typedef enum {CURRENT_HIGH, CURRENT_LOW} ActiveCurrent;
typedef enum {FULL_STEP, HALF_STEP} StepType;
typedef enum {CLOCKWISE, COUNTERCLOCK} ClockDirection;

struct controllerConfig {
	MotorState motorState;
	unsigned short frequency;
	unsigned short steps;
	ActiveCurrent activeCurrent;
	StepType stepType;
	ClockDirection clockDirection;
};
typedef struct controllerConfig ControllerConfig;

void ControllerManager_Init(void);
void ControllerManager_ProccessInputs(unsigned long key);
void ControllerManager_UpdateOutputs(void);
unsigned long ControllerManager_GetTaskDelay(void);

#endif /* CONTROLLERMANAGER_CONTROLLERMANAGER_H_ */
