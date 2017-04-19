#ifndef __UART_H
#define __UART_H

#include "stdint.h"


	


#define RX_CYCLIC_BUFFER_SIZE      0x100
#define RX_CYCLIC_BUFFER_SMASK     0xff


typedef struct{

	char name[10];
	char args[3][10];
	uint8_t argsNum;
	
}Command;

typedef struct 
{
	uint8_t RxCyclicBuffer[RX_CYCLIC_BUFFER_SIZE];
  uint8_t RxCyclicBufferIndex;
  uint8_t RxCyclicBufferLastLineIndex;	
  uint8_t RxCyclicBufferMark;
  char RxLine[RX_CYCLIC_BUFFER_SIZE];
  Command RxCommand;

}UartData;



void InitUsart(void);
void UartTask(void *op);
uint8_t prefix(const char *pre, const char *str);
uint8_t ParsCommand(char * line ,uint8_t length,Command *com);
void AnalyzyPcCommand(void);
void InitRxCommandTask(void);
void RxCommandTask(void *op);
int Str2Int(char *str, uint8_t length);

#endif

