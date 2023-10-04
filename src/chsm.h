//
// Created by shestakovvv on 04.10.2023.
//

#ifndef CHSM_CHSM_H
#define CHSM_CHSM_H

#include <stdbool.h>
#include <stdint.h>

typedef struct CHSM_State CHSM_State;

struct CHSM_State {
        CHSM_State* super;
        CHSM_State* Next;

        bool IsActive;
        uint32_t TickRate;

        void (*Entry)(void);
        void (*MainLoop)(void);
        void (*Exit)(void);
};

void CHSM_Create(void);
void CHSM_Start(void);

void CHSM_State_Run(CHSM_State *self);
void CHSM_State_Transfer(CHSM_State *self, CHSM_State *next);

uint32_t CHSM_GetTick(void);

#endif //CHSM_CHSM_H
