//
// Created by shestakovvv on 04.10.2023.
//

#include "chsm.h"

#define IF_SUPER if (self->super)

#define RUN_LIFECYCLE_FUN(fun) \
IF_SUPER { \
CHSM_State_##fun(self->super); \
} \
if (self->fun) { \
self->fun(); \
}


#define TICK_RATE_TO_TICKS(tickRate)       (1000 / tickRate)

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

void CHSM_State_Run(CHSM_Scheduler* scheduler) {
        if (!scheduler->Current)
                CHSM_State_Next(scheduler);

        CHSM_State_Entry(scheduler->Current);

        uint32_t tickStart = 0;
        uint32_t tickStop = TICK_RATE_TO_TICKS(scheduler->Current->TickRate);
        while(scheduler->Next == 0) {
                tickStart = CHSM_GetTick();

                CHSM_State_MainLoop(scheduler->Current);

                if (scheduler->Current->TickRate!=0)
                        while (CHSM_GetTick() - tickStart < tickStop);
        }

        CHSM_State_Exit(scheduler->Current);

        CHSM_State_Next(scheduler);
}

void CHSM_State_Entry(CHSM_State *self) {
        RUN_LIFECYCLE_FUN(Entry)
}

void CHSM_State_Exit(CHSM_State *self) {
        RUN_LIFECYCLE_FUN(Exit)
}

void CHSM_State_MainLoop(CHSM_State *self) {
        RUN_LIFECYCLE_FUN(MainLoop)
}

void CHSM_State_Transition(CHSM_Scheduler *scheduler, CHSM_State *state) {
        scheduler->Next = state;
}

void CHSM_State_Next(CHSM_Scheduler* scheduler) {
        scheduler->Current = scheduler->Next;
        scheduler->Next = 0;
}