/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f4xx_conf.h"
#include "nOS.h"

#define TASK1_STACK_SIZE    128
nOS_Thread task1;
nOS_Stack  task1_stack[TASK1_STACK_SIZE];

enum __taskPrio_e
{
    HIGHEST_PRIORITY,
    TASK1_PRIORITY  = 6,
}taskPrio_e;

void task1_thread(void *pArg);

int main(void)
{
    __disable_irq();
    nOS_Init();
    bsp_init();
    nOS_Start();

    __enable_irq();
    nOS_ThreadCreate(&task1, task1_thread, NULL, task1_stack, TASK1_STACK_SIZE, TASK1_PRIORITY);
    while(1)
    {

    }
}

void task1_thread(void *pArg)
{
    char *test = "Allo test \n";

    while(1)
    {
        uint8_t len = strlen(test);
        while(len)
        {
            USART_SendData(USART2, test+len);
            len --;
        }
        nOS_Sleep(100);
    }
}


void bsp_init()
{
    SysTick_Config(0.001 * SystemCoreClock);

    USART_InitTypeDef Uart;
    USART_ClockInitTypeDef UartClock;
    GPIO_InitTypeDef GPIO_init;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,    ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2,   ENABLE);

    ///USART 2 INIT - 115200 NO FLOW CONTROL
    GPIO_StructInit(&GPIO_init);
    GPIO_init.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_init.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA,&GPIO_init);

    Uart.USART_BaudRate = USART_HardwareFlowControl_None;
    Uart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &Uart);

    USART_ClockStructInit(&UartClock);
    USART_ClockInit(USART2,&UartClock);

}

SysTick_Handler()
{
    nOS_Tick();
}
