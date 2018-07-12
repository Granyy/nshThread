/*
 * user_thread.h
 *
 *  Created on: 12 juil. 2018
 *      Author: lgranier
 */

#ifndef USER_THREAD_H_
#define USER_THREAD_H_

#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_32.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

portTASK_FUNCTION_PROTO(th_blink, pvParameters);

#endif /* USER_THREAD_H_ */
