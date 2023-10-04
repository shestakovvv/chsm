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
        if (self->Entry != 0) {
                self->Entry();
        }
}

void CHSM_Exit(CHSM_State *self) {
        IF_SUPER {
                CHSM_Exit(self->super);
        }
        if (self->Exit != 0) {
                self->Exit();
        }
}

void CHSM_MainLoop(CHSM_State *self) {
        IF_SUPER {
                CHSM_MainLoop(self->super);
        }
        if (self->MainLoop != 0) {
                self->MainLoop();
        }
}

void CHSM_State_Next(CHSM_State* next) {
        CHSM_State_Run(next);
}

void CHSM_State_Run(CHSM_State *self) {
        self->IsActive = true;

        CHSM_Entry(self);

        uint32_t tickStart = 0;
        uint32_t tickStop = TICK_RATE_TO_TICKS(self->TickRate);
        while(self->IsActive) {
                tickStart = CHSM_GetTick();

                CHSM_MainLoop(self);

                if (self->TickRate!=0)
                        while (CHSM_GetTick() - tickStart < tickStop);
        }

        CHSM_Exit(self);

        CHSM_State_Next(self->Next);
}

void CHSM_State_Transfer(CHSM_State *self, CHSM_State *next) {
        self->IsActive = false;
        self->Next = next;
}