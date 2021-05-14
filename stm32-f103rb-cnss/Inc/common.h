/*
 * common.h
 *
 *  Created on: 21 March 2021
 *      Author: mayan and naomi
 */

#ifndef COMMON_H_
#define COMMON_H_

enum _bool{
	FALSE = 0,
	TRUE = 1
};
typedef enum _bool BOOL;

enum _state{
	PASS = 0,
	STANDBY = 1,
	FAIL = 2
};
typedef enum _state STATE;

enum _switch_state {
	OFF,
	ON,
	NO_PATH,
	NON
};
typedef enum _switch_state SWITCH_STATE;

#endif /* COMMON_H_ */
