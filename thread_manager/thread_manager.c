/*
 * thread_manager.c
 *
 *  Created on: 11 juil. 2018
 *      Author: lgranier
 */

#include "thread_manager.h"

static thread_array_t thread_array;

portTASK_FUNCTION_PROTO(th_run_shell, pvParameters);

void th_run_shell(void *pvParameters) {
	run_shell();
}

int thread_register(const char *name, thread_handler_t handler, int priority, int stack_allocated_size) {
	if (thread_array.count >= THREAD_MAX_COUNT)
		return THREAD_STATUS_MAX_THREAD_NB_REACH;

	size_t name_len = strlen(name);
	if (name_len == 0 || name_len > THREAD_NAME_MAX_STRING_SIZE || !handler)
		return THREAD_STATUS_UNSUPPORTED;

	strcpy(thread_array.array[thread_array.count].name, name);
	thread_array.array[thread_array.count].handler = handler;
	thread_array.array[thread_array.count].th_handler = NULL;
	thread_array.array[thread_array.count].status = THREAD_STATUS_STOPPED;
	thread_array.array[thread_array.count].priority = priority;
	thread_array.array[thread_array.count].stack_allocated_size = stack_allocated_size;

	thread_array.count++;

	return THREAD_STATUS_OK;
}

int thread_manager_start(void) {
	thread_array.count = 0;

	thread_register("blink", th_blink, 1, 128);

	shell_init();
	shell_register_command("start", cmd_start_handler);
	shell_register_command("stop", cmd_stop_handler);
	shell_register_command("thread", cmd_display_handler);

	xTaskCreate(th_run_shell, "shell", 2048, (void *) NULL, 1, (void **) NULL);
	vTaskStartScheduler();

	return SHELL_STATUS_OK;
}

int cmd_start_handler(int argc, char **argv) {
	if (argc > 1) {
		thread_t* thread_match = thread_find_matching(&thread_array, argv[1], strlen(argv[1]));
		if (thread_match != NULL) {
			if (thread_match->status != THREAD_STATUS_RUNNING) {
				printf("%s started\r\n", argv[1]);
				thread_match->status = THREAD_STATUS_RUNNING;
				xTaskCreate(thread_match->handler, thread_match->name, 512,
						(void *) NULL, thread_match->priority,
						&(thread_match->th_handler));
				return SHELL_STATUS_OK;
			} else {
				printf("%s is already running\r\n", argv[1]);
				return SHELL_STATUS_FAILURE;
			}
		} else {
			puts("ERROR: thread name not found");
			return SHELL_STATUS_FAILURE;
		}
	} else {
		puts("ERROR: You must specify a thread name");
		puts("usage: start <thread name> [<parameters>]");
		return SHELL_STATUS_FAILURE;
	}
}

int cmd_stop_handler(int argc, char **argv) {
	if (argc > 1) {
		thread_t* thread_match = thread_find_matching(&thread_array, argv[1],
				strlen(argv[1]));
		if (thread_match != NULL) {
			if (thread_match->status != THREAD_STATUS_STOPPED) {
				printf("%s stopped\r\n", argv[1]);
				vTaskDelete(thread_match->th_handler);
				thread_match->status = THREAD_STATUS_STOPPED;
				return SHELL_STATUS_OK;
			} else {
				printf("no thread named %s is running\r\n", argv[1]);
				return SHELL_STATUS_FAILURE;
			}
		} else {
			puts("ERROR: thread name not found");
			return SHELL_STATUS_FAILURE;
		}

	} else {
		puts("ERROR: You must specify a thread name");
		puts("usage: stop <thread name>");
		return SHELL_STATUS_FAILURE;
	}
}

int cmd_display_handler(int argc, char **argv) {
	printf("+---- THREAD MANAGER ----+\r\n");
	printf("| Name\t Running\t |\r\n");
	printf("+------------------------+\r\n");
	for (int i = 0; i < thread_array.count; i++) {
		printf("| %s\t %i\t\t |\r\n", thread_array.array[i].name, thread_array.array[i].status);
	}
	printf("+------------------------+\r\n");
	return SHELL_STATUS_OK;
}

thread_t* thread_find_matching(thread_array_t *cmds, const char *partial_name, int name_size) {
	for (int i = 0; i < cmds->count; i++)
		if (memcmp(partial_name, cmds->array[i].name, name_size) == 0)
			return &cmds->array[i];
	return NULL;
}
