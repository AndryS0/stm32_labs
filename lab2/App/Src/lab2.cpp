#include "lab2.h"
#include "gpio.h"
#include "tim.h"

static uint16_t led_r = 0, led_g = 0, led_b = 0;
const uint16_t inc_r = 5, inc_g = 10, inc_b = 30;
const uint16_t led_min = 5, led_max = 95;

static uint16_t led_wrap(uint16_t v)
{
    return (v > led_max) ? v - (led_max - led_min + 1) : v;
}

static uint32_t pct_to_ccr(uint8_t pct)
{
    uint32_t period = __HAL_TIM_GET_AUTORELOAD(PWM_TIMER) + 1;
    return period * (pct > 100 ? 100 : pct) / 100;
}

// in 0..100%
void rgb_set(uint8_t r, uint8_t g, uint8_t b)
{
    __HAL_TIM_SET_COMPARE(PWM_TIMER, TIM_CHANNEL_1, pct_to_ccr(r));
    __HAL_TIM_SET_COMPARE(PWM_TIMER, TIM_CHANNEL_2, pct_to_ccr(g));
    __HAL_TIM_SET_COMPARE(PWM_TIMER, TIM_CHANNEL_3, pct_to_ccr(b));
}

static void on_press(unsigned pin)
{
    switch (pin) {
    case JOYA_Pin:   led_b = led_wrap(led_b + inc_b);  break;  // ^
    case JOYB_Pin:   led_g = led_wrap(led_g + inc_g);  break;  // >
    case JOYC_Pin:   led_r = led_wrap(led_r + inc_r);  break;  // <
    case JOYD_Pin:                                     break;  // v
    case JOYCTR_Pin: led_r = led_g = led_b = led_max;  break;  // center
    default:                                           break;
    }
}

// each callback restarts settle window of button state
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t)
{
    __HAL_TIM_SET_COUNTER(&htim7, 0);
    // HAL_TIM_Base_Init raises an update event to load the prescaler, so UIF may
    // already be pending on the first start; without this the callback would
    // fire immediately instead of one period later.
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim7);
}

// this method invokes only when settle window passes, so states are trustworhy
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance != TIM7) return;
    HAL_TIM_Base_Stop_IT(&htim7);

    // last confirmed state of each button
    static bool held[JOY_COUNT] = {};

    for (unsigned i = 0; i < JOY_COUNT; i++) {
        const bool now_held =
            HAL_GPIO_ReadPin(JOY[i].port, JOY[i].pin) == GPIO_PIN_RESET; // active-low
        if (now_held && !held[i]) on_press(JOY[i].pin);
        held[i] = now_held;
    }

    rgb_set(led_r, led_g, led_b);
}


int main()
{
    HAL_Init();
    SystemClock_Config();
    generated_MPU_Config();
    MX_TIM7_Init();
    MX_TIM4_Init();
    MX_GPIO_Init();

    HAL_TIM_PWM_Start(PWM_TIMER, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(PWM_TIMER, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(PWM_TIMER, TIM_CHANNEL_3);
    while (1)
    {
        __WFI();
    }
}
