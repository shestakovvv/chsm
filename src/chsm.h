//
// Created by shestakovvv on 04.10.2023.
//

#ifndef CHSM_CHSM_H
#define CHSM_CHSM_H

#include <stdbool.h>
#include <stdint.h>

typedef struct CHSM_State CHSM_State;

typedef struct CHSM_Scheduler {
        CHSM_State *Current;
        CHSM_State *Next;
} CHSM_Scheduler;

struct CHSM_State {
        CHSM_State* super;

        uint32_t TickRate;

        void (*Entry)(void);
        void (*MainLoop)(void);
        void (*Exit)(void);
};

void CHSM_Create(void);
void CHSM_Start(void);

void CHSM_State_Run(CHSM_Scheduler* scheduler);
void CHSM_State_Transfer(CHSM_Scheduler *scheduler, CHSM_State *state);

uint32_t CHSM_GetTick(void);

#endif //CHSM_CHSM_H
