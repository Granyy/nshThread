/*
 * thread_manager.h
 *
 *  Created on: 11 juil. 2018
 *      Author: lgranier
 */

#ifndef THREAD_MANAGER_H_
#define THREAD_MANAGER_H_

#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_32.h"
#include "stdio.h"
#include "string.h"
#include "shell.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"
#include "user_thread.h"

#define THREAD_NAME_MAX_STRING_SIZE 12
#define THREAD_MAX_COUNT 5

typedef enum thread_manager_status {
	THREAD_STATUS_OK,
	THREAD_STATUS_FAILURE,
	THREAD_STATUS_UNSUPPORTED,
	THREAD_STATUS_MAX_THREAD_NB_REACH,
} thread_manager_status_t;

typedef enum thread_status {
	THREAD_STATUS_STOPPED,
	THREAD_STATUS_RUNNING
} thread_status_t;

typedef char thread_name_t[THREAD_NAME_MAX_STRING_SIZE];
typedef void (*thread_handler_t)(void *);

typedef struct thread {
	thread_handler_t handler;
	thread_name_t name;
	TaskHandle_t th_handler;
	thread_status_t status;
	int priority;
	int stack_allocated_size;
} thread_t;

typedef struct thread_array {
	thread_t array[THREAD_MAX_COUNT];
	int count;
} thread_array_t;

thread_t* thread_find_matching(thread_array_t *cmds, const char *partial_name, int name_size);
int thread_manager_start(void);
int cmd_start_handler(int argc, char **argv);
int cmd_stop_handler(int argc, char **argv);
int cmd_display_handler(int argc, char **argv);
int thread_register(const char *name, thread_handler_t handler, int priority, int stack_allocated_size);

#endif /* THREAD_MANAGER_H_ */
