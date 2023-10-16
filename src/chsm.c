//
// Created by shestakovvv on 04.10.2023.
//

#include "chsm.h"

static uint8_t StackFunctionCounter = 0;        // кол-во вызовов рекурсивных функций

void CHSM_State_Exit(CHSM_State *self);
void CHSM_State_Entry(CHSM_State *self);
void CHSM_State_MainLoop(CHSM_State *self);
void CHSM_State_Next(CHSM_Scheduler* scheduler);

void CHSM_Create(CHSM_Scheduler *self, CHSM_State *initialState) {
        self->IsActive = true;
        self->Next = 0;
        self->Current = initialState;
}

void CHSM_Run(CHSM_Scheduler* self) {
        while (self->IsActive) {
                CHSM_State_Run(self);
        }
}

void CHSM_State_Init(CHSM_State* self) {
        self->_startTick = CHSM_GetTick();
}

void CHSM_State_Run(CHSM_Scheduler* scheduler) {
        if (!scheduler->Current)
                return; // TODO: Error NO STATE

        CHSM_State_Init(scheduler->Current);

        StackFunctionCounter = 0;
        CHSM_State_Entry(scheduler->Current);

        while(scheduler->Next == 0) {
                StackFunctionCounter = 0;
                CHSM_State_MainLoop(scheduler->Current);
        }

        StackFunctionCounter = 0;
        CHSM_State_Exit(scheduler->Current);

        CHSM_State_Next(scheduler);
}

void CHSM_State_Entry(CHSM_State *self) {
        if (StackFunctionCounter >= CHSM_MAX_STACK_COUNT)
                return; // StackOverflow;
        if (self->Super) {
                StackFunctionCounter++;
                CHSM_State_Entry(self->Super);
        } if (self->Entry) {
                self->Entry();
        }
}

void CHSM_State_Exit(CHSM_State *self) {
        if (StackFunctionCounter >= CHSM_MAX_STACK_COUNT)
                return; // StackOverflow;
        if (self->Super) {
                StackFunctionCounter++;
                CHSM_State_Exit(self->Super);
        } if (self->Exit) {
                self->Exit();
        }
}

void CHSM_State_MainLoop(CHSM_State *self) {
        if (StackFunctionCounter >= CHSM_MAX_STACK_COUNT)
                return; // StackOverflow;
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
}

void CHSM_State_Transition(CHSM_Scheduler *scheduler, CHSM_State *state) {
        scheduler->Next = state;
}

void CHSM_State_Next(CHSM_Scheduler* scheduler) {
        scheduler->Current = scheduler->Next;
        scheduler->Next = 0;
}