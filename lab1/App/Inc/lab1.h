#ifndef LAB1_H
#define LAB1_H

#include "main.h"
struct LED
{
    GPIO_TypeDef* port;
    unsigned pin;
};

const LED LEDS[] = {
    {LED1_GPIO_Port, LED1_Pin},
    {LED2_GPIO_Port, LED2_Pin},
    {LED3_GPIO_Port, LED3_Pin},
    {LED4_GPIO_Port, LED4_Pin}
};
const unsigned LED_COUNT = sizeof(LEDS) / sizeof(LED);

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

// current task state enum
enum class State : unsigned
{
    Stopped, LeftRun, RightRun
};

extern "C" void SystemClock_Config(void);

#endif
