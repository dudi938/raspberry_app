#include "uart.h"
#include "gpio.h"
#include "platform.h"
#include "platform.h"
#include "task_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "motor_control.h"
#include "robot_main_module.h"



#define FAILURE 0
#define MIN_LINE_LENGTH 4

TaskHandle UartTaskID;

UartData uartInfo;

//TODO make static or remove
UART_HandleTypeDef huart1;




char* stringToBinary(char* s) ;
void binchar(char output[],  char character);
void itoa(int value, char* str, int base);



/* USART init function */
void InitUsart(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct;
	
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED ;
  
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&huart1);
	
	// set rx it enable
	USART1->CR1 |= USART_CR1_RXNEIE;
	
	InitRxCommandTask();
}


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1)
  {
    __USART1_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
	GPIO_InitStruct.Pin = UART1_TX_PIN | UART1_RX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(UART1_PORT, &GPIO_InitStruct);
	

  /* System interrupt init*/
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}











void USART1_IRQHandler(void)
{
		HAL_UART_Receive(&huart1, &uartInfo.RxCyclicBuffer[uartInfo.RxCyclicBufferIndex], 1, 500);
		uartInfo.RxCyclicBufferIndex = ((uartInfo.RxCyclicBufferIndex + 1) & RX_CYCLIC_BUFFER_SMASK);
}



uint8_t buff[128];
uint8_t currSize;
int fputc(int ch, FILE *f)
{
	buff[currSize++] = (uint8_t)ch;
	
  if(ch == '\n'){
		
		HAL_UART_Transmit(&huart1, buff, currSize, 100);
	
		currSize = 0;
	}
	
  return(ch);
}




//init  command recieve task
void InitRxCommandTask(void)
{		
	UartTaskID = add_task(&RxCommandTask);
	exec_task(UartTaskID, 50, 1, (void*) NULL);		

	uartInfo.RxCyclicBuffer[0] = '\r';
	uartInfo.RxCyclicBuffer[1] = '\n';
	uartInfo.RxCyclicBufferIndex = 2;
	uartInfo.RxCyclicBufferLastLineIndex = 2;
	uartInfo.RxCyclicBufferMark = 2;
}



void RxCommandTask(void *op)
{
	for(;uartInfo.RxCyclicBufferMark != uartInfo.RxCyclicBufferIndex; uartInfo.RxCyclicBufferMark == RX_CYCLIC_BUFFER_SMASK ?uartInfo.RxCyclicBufferMark = 0 :uartInfo.RxCyclicBufferMark++)
	{
		
		uint8_t LineLength;
		if(uartInfo.RxCyclicBuffer[uartInfo.RxCyclicBufferMark] == '\n'  &&  uartInfo.RxCyclicBuffer[uartInfo.RxCyclicBufferMark - 1] == '\r')
		{
			// new line recieved.
			
			memset(uartInfo.RxLine,0,RX_CYCLIC_BUFFER_SIZE);
			
			if(uartInfo.RxCyclicBufferLastLineIndex > uartInfo.RxCyclicBufferIndex)
			{
				uint8_t LengthInTail = RX_CYCLIC_BUFFER_SIZE - uartInfo.RxCyclicBufferLastLineIndex;
				uint8_t LengthInHead = uartInfo.RxCyclicBufferMark + 1;
				LineLength = LengthInTail + LengthInHead;
				
				strncpy(uartInfo.RxLine,(char *)&uartInfo.RxCyclicBuffer[uartInfo.RxCyclicBufferLastLineIndex],LengthInTail);
				strncpy(uartInfo.RxLine + LengthInTail,(char *)&uartInfo.RxCyclicBuffer[0],LengthInHead);								
						// need copy the line from the tail of the buffer.
			}
			
			else
			{
				  LineLength = uartInfo.RxCyclicBufferMark - uartInfo.RxCyclicBufferLastLineIndex + 1;
					strncpy(uartInfo.RxLine,(char *)&uartInfo.RxCyclicBuffer[uartInfo.RxCyclicBufferLastLineIndex],LineLength);
					
			}
			uartInfo.RxCyclicBufferLastLineIndex = uartInfo.RxCyclicBufferIndex;
			
			
			if(ParsCommand(uartInfo.RxLine,LineLength, &uartInfo.RxCommand) == SUCCESS)
			{
				AnalyzyPcCommand();
			}
			
		}
	}
}


void AnalyzyPcCommand(void)
{
		if(!strcmp(uartInfo.RxCommand.name,"GF"))
		{
			printf("<-GF\r\n");
			
			int power = Str2Int(uartInfo.RxCommand.args[0], strlen(uartInfo.RxCommand.args[0]));
			RobotMove(etForDirect,power);	
		}
		
		else if(!strcmp(uartInfo.RxCommand.name,"GB"))
		{
			  printf("<-GB\r\n");	
			
			
				int power = Str2Int(uartInfo.RxCommand.args[0], strlen(uartInfo.RxCommand.args[0]));
				RobotMove(etBackDirect,power);				
		}
		else if(!strcmp(uartInfo.RxCommand.name,"GR"))
		{
			  printf("<-GR\r\n");	
			
			
				int power = Str2Int(uartInfo.RxCommand.args[0], strlen(uartInfo.RxCommand.args[0]));
				RobotMove(etForRight,power);				
		}
		else if(!strcmp(uartInfo.RxCommand.name,"GL"))
		{
			  printf("<-GL\r\n");	
			
			
				int power = Str2Int(uartInfo.RxCommand.args[0], strlen(uartInfo.RxCommand.args[0]));
				RobotMove(etForLeft,power);				
		}		
		else if(!strcmp(uartInfo.RxCommand.name,"ST"))
		{
				printf("<-ST\r\n");
				RobotMove(etRobotStop,0);
		}		

}
uint8_t ParsCommand(char * line ,uint8_t length,Command *com)
{
	memset(&uartInfo.RxCommand,0,sizeof(uartInfo.RxCommand));

	if(strlen(line) < 4)
	{
		return FAILURE;
	}


	if(strlen(line) == MIN_LINE_LENGTH)
	{
		printf("life\r\n");
		return SUCCESS;
	}

	
	if(prefix("AT+",line))
	{
		char *NameStart = strchr(line,'+');
	  char *NameEnd   = strchr(line,'=');
		
		
		com->argsNum = 0;
		
		if(NameEnd)//check if have a parameters
		{
			strncpy(com->name,line + (NameStart - line) + 1,NameEnd - NameStart - 1);
			

			// cut the \r\n
			line[length-1] = line[length-2] = NULL;
			
			char *ArgEnd = strtok(line + (uint8_t)(NameEnd - line) +1,",");
			
			while(ArgEnd != NULL)
			{
					strcpy(com->args[com->argsNum],ArgEnd);
					com->argsNum++;   
					ArgEnd = strtok (NULL, ",");			
			}
		}
		
		else
		{
			// CMD without parameters
			strncpy(com->name,line + (NameStart - line) + 1,length - (NameStart - line) - 3);
		}

		return SUCCESS;
		
	}
	
	
	
	
}


//checks if pre is prefix of str
uint8_t prefix(const char *pre, const char *str)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}


int Str2Int(char *str, uint8_t length)
{
		int offset = length;
		int num = 0;
		while(*str)
		{
			if((*str >= '0') && (*str <= '9'))
			{
				volatile int p = pow(10, (double)(offset - 1));
				num += ((*str) - 0x30) * pow(10, (double)(offset - 1));
				str++;
				offset--;
			}
			else return 0;
		}
		return num;
}




#if 0
void swap(int* a, int* b)
{
 int* temp = a;
 a = b;
 b = temp;
}



/* A utility function to reverse a string  */
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap((int *)(str+start), (int *)(str+end));
        start++;
        end--;
    }
}

// Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}


void printstringasbinary(char* s)
{
    // A small 9 characters buffer we use to perform the conversion
    char output[9];

    // Until the first character pointed by s is not a null character
    // that indicates end of string...
    while (*s)
    {
        // Convert the first character of the string to binary using itoa.
        // Characters in c are just 8 bit integers, at least, in noawdays computers.
        itoa(*s, output, 2);

        // print out our string and let's write a new line.
        puts(output);

        // we advance our string by one character,
        // If our original string was "ABC" now we are pointing at "BC".
        ++s;
    }
}
#else





char* stringToBinary(char* s) {
    if(s == NULL) return 0; /* no input string */
    size_t len = strlen(s);
    char *binary = malloc(len*8 + 1); // each char is one byte (8 bits) and + 1 at the end for null terminator
    int i =0;

    char output[9];
    for(i=0; i< len; i++){
        binchar(output,  s[i]);
        strcat(binary,output);
    }
    return binary;
}


void binchar(char output[],  char character)
{
    //char output[9];
    itoa(character, output, 2);
}



// since GCC is not fully supporting itoa function here is its implementaion
// itoa implementation is copied from here http://www.strudel.org.uk/itoa/

void itoa(int value, char* str, int base) {

    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";

    char* wstr=str;

    int sign;




    // Validate base

    if (base<2 || base>35){ *wstr='\0'; return; }



    // Take care of sign

    if ((sign=value) < 0) value = -value;




    // Conversion. Number is reversed.

    do *wstr++ = num[value%base]; while(value/=base);

    if(sign<0) *wstr++='-';

    *wstr='\0';



    // Reverse string
void strreverse(char* begin, char* end);
    strreverse(str,wstr-1);

}

void strreverse(char* begin, char* end) {

    char aux;

    while(end>begin)

        aux=*end, *end--=*begin, *begin++=aux;

}
#endif