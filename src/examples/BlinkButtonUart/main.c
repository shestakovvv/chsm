#include "platform.h"
#include "gpio.h"
#include "usart1.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "chsm.h"

void State_Init_MainLoop(void);
void State_Active_OnEntry(void);
void State_Active_MainLoop(void);
void State_Active_OnExit(void);
void State_Active2_OnEntry(void);
void State_Active2_MainLoop(void);
void State_Active2_OnExit(void);

CHSM_Scheduler Scheduler;

CHSM_State State_Init = {
        .TicksDelay = 100,
        .Entry = 0,
        .Exit = 0,
        .MainLoop = &State_Init_MainLoop
};

CHSM_State State_Active = {
        .Super = &State_Init,
        .TicksDelay = 100,
        .Entry = &State_Active_OnEntry,
        .Exit = &State_Active_OnExit,
        .MainLoop = &State_Active_MainLoop
};

CHSM_State State_Active2 = {
        .Super = &State_Active,
        .TicksDelay = 1000,
        .Entry = &State_Active2_OnEntry,
        .Exit = &State_Active2_OnExit,
        .MainLoop = &State_Active2_MainLoop
};

int main() {
        Platform_Create();

        GPIO_Init();
        USART1_UART_Init(&huart1);

        CHSM_Create(&Scheduler, &State_Init);
        CHSM_Result res = CHSM_Run(&Scheduler);
        if (res != (CHSM_Result)RESULT_OK) {
                Platform_ErrorIndicator(PLATFORM_INDICATOR_ON);
        }
}

void State_Init_MainLoop(void) {
        uint8_t res = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);
        if (res) {
                if (Scheduler.Current == &State_Init)
                        CHSM_State_Transition(&Scheduler, &State_Active2);
                else
                        CHSM_State_Transition(&Scheduler, &State_Init);
        }

        Platform_MainIndicator(PLATFORM_INDICATOR_TOGGLE);
}

void State_Active_MainLoop(void) {
        Platform_TxIndicator(PLATFORM_INDICATOR_TOGGLE);
}

void State_Active_OnEntry(void) {
        uint8_t msg[] = "Entry Active\n";
        Platform_TransmitMessage(msg, strlen((char*)msg));
}

void State_Active_OnExit(void) {
        uint8_t msg[] = "Exit Active\n";
        Platform_TransmitMessage(msg, strlen((char*)msg));
        Platform_TxIndicator(PLATFORM_INDICATOR_OFF);
}

void State_Active2_OnEntry(void) {
        uint8_t msg[] = "Entry Active2\n";
        Platform_TransmitMessage(msg, strlen((char*)msg));
        Platform_RxIndicator(PLATFORM_INDICATOR_TOGGLE);
}

void State_Active2_MainLoop(void) {
        uint8_t msg[] = "Main Loop Active2\n";
        Platform_TransmitMessage(msg, strlen((char*)msg));
        Platform_RxIndicator(PLATFORM_INDICATOR_TOGGLE);
}

void State_Active2_OnExit(void) {
        uint8_t msg[] = "Exit Active2\n";
        Platform_TransmitMessage(msg, strlen((char*)msg));
        Platform_RxIndicator(PLATFORM_INDICATOR_OFF);
}

uint32_t CHSM_GetTick(void) {
        return HAL_GetTick();
}

