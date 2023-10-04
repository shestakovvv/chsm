//
// Created by shestakovvv on 04.10.2023.
//

#include "chsm.h"

#define IF_SUPER if (self->super)

#define RUN_LIFECYCLE_FUN(fun) \
IF_SUPER { \
if (self->super->fun != 0) { \
self->super->fun(); \
} \
} \
if (self->fun != 0) { \
self->fun(); \
}

#define TICK_RATE_TO_TICKS(tickRate)       (1000 / tickRate)

void CHSM_Create(void);

void CHSM_Start(void);

void CHSM_Entry(CHSM_State *self) {
        IF_SUPER {
                CHSM_Entry(self->super);
        }
        if (self->Entry) {
                self->Entry();
        }
}

void CHSM_Exit(CHSM_State *self) {
        IF_SUPER {
                CHSM_Exit(self->super);
        }
        if (self->Exit) {
                self->Exit();
        }
}

void CHSM_MainLoop(CHSM_State *self) {
        IF_SUPER {
                CHSM_MainLoop(self->super);
        }
        if (self->MainLoop) {
                self->MainLoop();
        }
}

void CHSM_State_Next(CHSM_Scheduler* scheduler) {
        scheduler->Current = scheduler->Next;
        scheduler->Next = 0;
        CHSM_State_Run(scheduler);
}

void CHSM_State_Run(CHSM_Scheduler* scheduler) {
        if (!scheduler->Current)
                CHSM_State_Next(scheduler);

        CHSM_Entry(scheduler->Current);

        uint32_t tickStart = 0;
        uint32_t tickStop = TICK_RATE_TO_TICKS(scheduler->Current->TickRate);
        while(scheduler->Next == 0) {
                tickStart = CHSM_GetTick();

                CHSM_MainLoop(scheduler->Current);

                if (scheduler->Current->TickRate!=0)
                        while (CHSM_GetTick() - tickStart < tickStop);
        }

        CHSM_Exit(scheduler->Current);

        CHSM_State_Next(scheduler);
}

void CHSM_State_Transfer(CHSM_Scheduler *scheduler, CHSM_State *state) {
        scheduler->Next = state;
}