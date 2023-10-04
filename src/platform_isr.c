//
// Created by shestakovvv on 05.09.2023.
//
#include "stm32f3xx_hal.h"
#include "usart1.h"

void NMI_Handler(void)
{
        while (1)
        {
        }
}

void HardFault_Handler(void)
{
        while (1)
        {
        }
}

void MemManage_Handler(void)
{
        while (1)
        {
        }
}

void BusFault_Handler(void)
{
        while (1)
        {
        }
}

void UsageFault_Handler(void)
{
        while (1)
        {
        }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
        HAL_IncTick();
}

void USART1_IRQHandler(void)
{
        HAL_UART_IRQHandler(&huart1);
}