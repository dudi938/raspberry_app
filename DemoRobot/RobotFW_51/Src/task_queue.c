#include "task_queue.h"
#include "stm32f0xx_hal.h"

typedef struct{
	task_cb cb;
	void *opaq;
	uint32_t period;
	uint32_t execTime;
	uint8_t exec;	
}Task;

static volatile Task tasks[20];
static volatile uint8_t tasksNum = 0;

void init_tasks()
{
	
}

void event_loop()
{
	
	while(1){
		
		uint32_t ticks = HAL_GetTick();
		
		uint32_t closestTime = 0xffffffff;
		
		for(uint8_t idx=0; idx < tasksNum; idx++){
			
			volatile Task *entry = tasks + idx;
			
			if(entry->exec && ticks >= entry->execTime){
			
				if(!entry->period){
					entry->exec=0;
				}
				
				if(entry->cb){
					entry->cb(entry->opaq);
				}
				
				if(entry->period){
					entry->execTime = ticks + entry->period;
				}
			}
			
			if(entry->period){
				if(entry->execTime < closestTime){
					closestTime = entry->execTime;
				}
			}
			
		}		
	}	
}

TaskHandle add_task(task_cb cb)
{
	
	Task task;
	task.cb = cb;
	task.exec = 0;
	
	tasks[tasksNum] = task;
	
	return (TaskHandle)(&tasks[tasksNum++]);
}

uint8_t exec_task(TaskHandle taskHandle, uint32_t period, uint8_t repeat, void *opaq)
{
	
	Task *entry = (Task*)taskHandle;
	
	if(entry->exec){
		//busy task
		return 1;
	}
	
	entry->opaq = opaq;

	if(repeat){
		entry->period = period;
		entry->execTime = HAL_GetTick() + period;
	}else{
		entry->period = 0;
		entry->execTime = HAL_GetTick() + period;
	}
	
	entry->exec = 1;
	
	return 0 ;
}

void cancel_task(TaskHandle taskHandle)
{

	Task *entry = (Task*)taskHandle;

	entry->exec = 0;
}


