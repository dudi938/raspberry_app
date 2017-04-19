#ifndef __TASK_QUEUE_H
#define __TASK_QUEUE_H
#include "stdint.h"

#define MAX_LISTENERS 10

typedef void (*task_cb)(void *opaq);

typedef int TaskHandle;


#define Priority0  50
#define Priority1  100
#define Priority2  250
#define Priority3  500
#define Priority4  1000



void init_tasks(void);
TaskHandle add_task(task_cb cb);
uint8_t exec_task(TaskHandle taskHandle, uint32_t period, uint8_t repeat, void *opaq);
void cancel_task(TaskHandle taskHandle);
void event_loop(void);

#endif
