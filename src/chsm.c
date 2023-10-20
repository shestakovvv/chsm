//
// Created by shestakovvv on 04.10.2023.
//

#include "chsm.h"

static uint8_t StackFunctionCounter = 0;        // кол-во вызовов рекурсивных функций

CHSM_Result CHSM_State_Exit(CHSM_State *self);
CHSM_Result CHSM_State_Entry(CHSM_State *self);
CHSM_Result CHSM_State_MainLoop(CHSM_State *self);
CHSM_Result CHSM_State_Next(CHSM_Scheduler* scheduler);

CHSM_Result CHSM_Create(CHSM_Scheduler *self, CHSM_State *initialState) {
        if (!initialState) {
                return CHSM_RESULT_ERROR_EMPTY_STATE;
        }

        self->IsActive = true;
        self->Next = 0;
        self->Current = initialState;

        return (CHSM_Result)RESULT_OK;
}

CHSM_Result CHSM_Run(CHSM_Scheduler* self) {
        while (self->IsActive) {
                CHSM_Result result = CHSM_State_Run(self);
                if (result != (CHSM_Result)RESULT_OK) {
                        return result;
                }
        }
        return (CHSM_Result)RESULT_OK;
}

void CHSM_State_Init(CHSM_State* self) {
        self->_startTick = CHSM_GetTick();
}

CHSM_Result CHSM_State_Run(CHSM_Scheduler* scheduler) {
        if (!scheduler->Current)
                return CHSM_RESULT_ERROR_EMPTY_STATE;

        CHSM_State_Init(scheduler->Current);

        CHSM_Result res;
        StackFunctionCounter = 0;
        if ((res = CHSM_State_Entry(scheduler->Current)) != (CHSM_Result)RESULT_OK) {
                return res;
        }

        while(scheduler->Next == 0) {
                StackFunctionCounter = 0;
                if ((res = CHSM_State_MainLoop(scheduler->Current)) != (CHSM_Result)RESULT_OK) {
                        return res;
                }
        }

        StackFunctionCounter = 0;
        if ((res = CHSM_State_Exit(scheduler->Current)) != (CHSM_Result)RESULT_OK) {
                return res;
        }

        return CHSM_State_Next(scheduler);
}

CHSM_Result CHSM_State_Entry(CHSM_State *self) {
        if (StackFunctionCounter >= CHSM_MAX_STACK_COUNT)
                return CHSM_RESULT_ERROR_STACK_OVERFLOW;

        if (self->Super) {
                StackFunctionCounter++;
                CHSM_State_Entry(self->Super);
        } if (self->Entry) {
                self->Entry();
        }

        // Вызов второй раз, чтобы вернуть результат в функциях, которые уже в стеке
        if (StackFunctionCounter >= CHSM_MAX_STACK_COUNT)
                return CHSM_RESULT_ERROR_STACK_OVERFLOW;

        return (CHSM_Result)RESULT_OK;
}

CHSM_Result CHSM_State_Exit(CHSM_State *self) {
        if (StackFunctionCounter >= CHSM_MAX_STACK_COUNT)
                return CHSM_RESULT_ERROR_STACK_OVERFLOW;
        if (self->Super) {
                StackFunctionCounter++;
                CHSM_State_Exit(self->Super);
        } if (self->Exit) {
                self->Exit();
        }

        // Вызов второй раз, чтобы вернуть результат в функциях, которые уже в стеке
        if (StackFunctionCounter >= CHSM_MAX_STACK_COUNT)
                return CHSM_RESULT_ERROR_STACK_OVERFLOW;

        return (CHSM_Result)RESULT_OK;
}

CHSM_Result CHSM_State_MainLoop(CHSM_State *self) {
        if (StackFunctionCounter >= CHSM_MAX_STACK_COUNT)
                return CHSM_RESULT_ERROR_STACK_OVERFLOW;
        if (self->Super) {
                StackFunctionCounter++;
                CHSM_State_MainLoop(self->Super);
        } if (self->MainLoop) {
                if (self->TicksDelay != 0) {
                        if (CHSM_GetTick() - self->_startTick > self->TicksDelay) {
                                self->MainLoop();
                                self->_startTick = CHSM_GetTick();
                        }
                } else {
                        self->MainLoop();
                }
        }

        // Вызов второй раз, чтобы вернуть результат в функциях, которые уже в стеке
        if (StackFunctionCounter >= CHSM_MAX_STACK_COUNT)
                return CHSM_RESULT_ERROR_STACK_OVERFLOW;

        return (CHSM_Result)RESULT_OK;
}

CHSM_Result CHSM_State_Transition(CHSM_Scheduler *scheduler, CHSM_State *state) {
        if (!state)
                return CHSM_RESULT_ERROR_EMPTY_STATE;
        scheduler->Next = state;

        return (CHSM_Result)RESULT_OK;
}

CHSM_Result CHSM_State_Next(CHSM_Scheduler* scheduler) {
        if (!scheduler->Next)
                return CHSM_RESULT_ERROR_EMPTY_STATE;

        scheduler->Current = scheduler->Next;
        scheduler->Next = 0;

        return (CHSM_Result)RESULT_OK;
}