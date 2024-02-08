//
// Created by shestakovvv on 04.10.2023.
//

#ifndef CHSM_CHSM_H
#define CHSM_CHSM_H

#include <stdbool.h>
#include <stdint.h>

typedef struct CHSM_State CHSM_State;

typedef struct CHSM_Scheduler {
        bool IsActive;

        CHSM_State *Current;
        CHSM_State *Next;
} CHSM_Scheduler;

#ifndef CHSM_MAX_STACK_COUNT
#define CHSM_MAX_STACK_COUNT 10
#endif

typedef enum CHSM_Result_s {
        CHSM_RESULT_OK = 0,
        CHSM_RESULT_ERROR,
        CHSM_RESULT_ERROR_ARGS,
        CHSM_RESULT_ERROR_STACK_OVERFLOW,
        CHSM_RESULT_ERROR_EMPTY_STATE
} CHSM_Result;

struct CHSM_State {
        CHSM_State* Super;

        uint32_t TicksDelay;

        uint32_t _startTick;

        void (*Entry)(void);
        void (*MainLoop)(void);
        void (*Exit)(void);
};

CHSM_Result CHSM_Create(CHSM_Scheduler *self, CHSM_State *initialState);
CHSM_Result CHSM_Run(CHSM_Scheduler* self);

void CHSM_State_Init(CHSM_State* scheduler);
CHSM_Result CHSM_State_Run(CHSM_Scheduler* scheduler);
CHSM_Result CHSM_State_Transition(CHSM_Scheduler *scheduler, CHSM_State *state);

uint32_t CHSM_GetTick(void);

#endif //CHSM_CHSM_H
