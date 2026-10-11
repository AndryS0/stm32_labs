#ifndef LAB2_H
#define LAB2_H

#include "main.h"
#include "tim.h"

struct Button
{
    GPIO_TypeDef* port;
    unsigned pin;
};

const Button JOY[] = {
    {JOYA_GPIO_Port,   JOYA_Pin  },
    {JOYB_GPIO_Port,   JOYB_Pin  },
    {JOYC_GPIO_Port,   JOYC_Pin  },
    {JOYD_GPIO_Port,   JOYD_Pin  },
    {JOYCTR_GPIO_Port, JOYCTR_Pin}
};
const unsigned JOY_COUNT = sizeof(JOY) / sizeof(Button);

extern "C" void SystemClock_Config(void);

const auto PWM_TIMER = &htim4;

#endif
