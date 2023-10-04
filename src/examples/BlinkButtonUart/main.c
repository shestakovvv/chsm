#include "platform.h"
#include "gpio.h"
#include "usart1.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "chsm.h"

#define UART_TX_BUFFER_SIZE (100U)
uint8_t UartTxBuffer[100];

void State_Init_MainLoop(void);
void State_Active_MainLoop(void);
void State_Active_OnExit(void);
void State_Active2_MainLoop(void);
void State_Active2_OnExit(void);

CHSM_Scheduler Scheduler;

CHSM_State State_Init = {
        .TickRate = 100,
        .Entry = 0,
        .Exit = 0,
        .MainLoop = &State_Init_MainLoop
};

CHSM_State State_Active = {
        .super = &State_Init,
        .TickRate = 100,
        .Entry = 0,
        .Exit = &State_Active_OnExit,
        .MainLoop = &State_Active_MainLoop
};

CHSM_State State_Active2 = {
        .super = &State_Active,
        .TickRate = 100,
        .Entry = 0,
        .Exit = &State_Active2_OnExit,
        .MainLoop = &State_Active2_MainLoop
};

int main() {
        Platform_Create();

        GPIO_Init();
        USART1_UART_Init(&huart1);

        memset(UartTxBuffer, 0, UART_TX_BUFFER_SIZE);

        CHSM_State_Transfer(&Scheduler,&State_Init);
        CHSM_State_Run(&Scheduler);
}

#define MSG_SIZE 10
char msg[] = "Hello mir\n";

void State_Init_MainLoop(void) {
        uint8_t res = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
        if (res) {
                if (Scheduler.Current == &State_Init)
                        CHSM_State_Transfer(&Scheduler, &State_Active2);
                else
                        CHSM_State_Transfer(&Scheduler, &State_Init);
        }

        memcpy(UartTxBuffer, msg, MSG_SIZE);
        Platform_TransmitMessage(UartTxBuffer, MSG_SIZE);
        memset(UartTxBuffer, 0, UART_TX_BUFFER_SIZE);

        Platform_MainIndicator(PLATFORM_INDICATOR_TOGGLE);
}

void State_Active_MainLoop(void) {
        Platform_TxIndicator(PLATFORM_INDICATOR_TOGGLE);
}

void State_Active_OnExit(void) {
        Platform_TxIndicator(PLATFORM_INDICATOR_OFF);
}

void State_Active2_MainLoop(void) {
        Platform_RxIndicator(PLATFORM_INDICATOR_TOGGLE);
}

void State_Active2_OnExit(void) {
        Platform_RxIndicator(PLATFORM_INDICATOR_OFF);
}

uint32_t CHSM_GetTick(void) {
        return HAL_GetTick();
}

