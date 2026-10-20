#ifndef LAB2_H
#define LAB2_H

#include "main.h"
#include "tim.h"

struct Button
{
    GPIO_TypeDef* port;
    unsigned pin;
};

extern "C" void SystemClock_Config(void);

const auto PWM_TIMER = &htim4;

#endif
