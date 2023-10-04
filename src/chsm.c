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

void CHSM_State_Run(CHSM_State *self) {
        self->IsActive = true;
        RUN_LIFECYCLE_FUN(Entry)

        uint32_t tickStart = 0;
        uint32_t tickStop = TICK_RATE_TO_TICKS(self->TickRate);
        while(self->IsActive) {
                tickStart = CHSM_GetTick();
                RUN_LIFECYCLE_FUN(MainLoop)

                if (self->TickRate!=0)
                        while (CHSM_GetTick() - tickStart < tickStop);
        }

        RUN_LIFECYCLE_FUN(Exit)

        CHSM_State_Run(self->Next);
}

void CHSM_State_Transfer(CHSM_State *self, CHSM_State *next) {
        self->IsActive = false;
        self->Next = next;
}