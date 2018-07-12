/*
 * user_thread.c
 *
 *  Created on: 12 juil. 2018
 *      Author: lgranier
 */

#include "user_thread.h"

void th_blink(void *pvParameters) {
	while (1) {
		HAL_GPIO_TogglePin(LED3_GPIO_PORT, LED3_PIN);
		vTaskDelay(1000);
	}
}
