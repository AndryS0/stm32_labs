#include "lab2.h"
#include "adc.h"
#include "dma.h"
#include "gpio.h"
#include "tim.h"
#include "salgs.h"

constexpr uint16_t adc_nch        = 2;                       // channels in scan sequence
constexpr uint16_t adc_idx_g      = 0;                       // rank 1 = A2
constexpr uint16_t adc_idx_b      = 1;                       // rank 2 = A3
constexpr uint16_t samples_per_ch = 16;                      // median window per channel
constexpr uint16_t half_len       = adc_nch * samples_per_ch;
constexpr uint16_t adc_buf_len    = 2 * half_len;

constexpr uint32_t adc_full_scale = 4096;
constexpr uint32_t adc_hyst       = 40;                      // hysteresis

constexpr uint8_t led_min_pct = 5;
constexpr uint8_t led_max_pct = 95;
constexpr uint8_t g_step_pct  = 10;
constexpr uint8_t b_step_pct  = 15;
constexpr uint8_t g_levels    = (led_max_pct - led_min_pct) / g_step_pct + 1;  // 10 levels
constexpr uint8_t b_levels    = (led_max_pct - led_min_pct) / b_step_pct + 1;  //  7 levels
constexpr uint8_t led_r_pct   = 0;                           // R channel is not used in this task

struct pot_ctl_t {
    uint8_t levels;     // num of brightness levels
    uint8_t step_pct;   // brightness change per level
    uint8_t level;      // current level (0 <= level < levels)
};

static pot_ctl_t pot_g = { g_levels, g_step_pct, 0 };
static pot_ctl_t pot_b = { b_levels, b_step_pct, 0 };

static uint8_t led_g = led_min_pct;
static uint8_t led_b = led_min_pct;

uint16_t adc_readings[adc_buf_len];

static uint32_t pct_to_ccr(uint8_t pct)
{
    uint32_t period = __HAL_TIM_GET_AUTORELOAD(PWM_TIMER) + 1;
    return period * (pct > 100 ? 100 : pct) / 100;
}

void rgb_set(uint8_t r, uint8_t g, uint8_t b)
{
    __HAL_TIM_SET_COMPARE(PWM_TIMER, TIM_CHANNEL_1, pct_to_ccr(r));
    __HAL_TIM_SET_COMPARE(PWM_TIMER, TIM_CHANNEL_2, pct_to_ccr(g));
    __HAL_TIM_SET_COMPARE(PWM_TIMER, TIM_CHANNEL_3, pct_to_ccr(b));
}

static uint8_t pot_update(pot_ctl_t *p, uint16_t adc)
{
    uint32_t n      = p->levels;
    uint32_t scaled = (uint32_t)adc * n;
    uint32_t lo     = (uint32_t)p->level * adc_full_scale;  // bottom edge of current zone
    uint32_t hi     = lo + adc_full_scale;                  // top edge (excl)
    uint32_t h      = adc_hyst * n;

    // change level with histeresis
    if (scaled + h < lo || scaled >= hi + h) {
        uint32_t lvl = scaled / adc_full_scale;
        p->level = (uint8_t)(lvl < n ? lvl : n - 1u);
    }
    return (uint8_t)(led_min_pct + p->level * p->step_pct);
}

void process(ADC_HandleTypeDef *hadc, const uint16_t *src, uint16_t len)
{
    uint16_t g_raw[samples_per_ch];
    uint16_t b_raw[samples_per_ch];
    uint16_t n = len / adc_nch;

    if (hadc->Instance != ADC1)
        return;
    if (n > samples_per_ch)
        n = samples_per_ch;

    // G,B,G,B,...
    for (uint16_t i = 0; i < n; i++) {
        g_raw[i] = src[i * adc_nch + adc_idx_g];
        b_raw[i] = src[i * adc_nch + adc_idx_b];
    }

    // median in 0..4095 range
    uint16_t g_med = median_u16(g_raw, n);
    uint16_t b_med = median_u16(b_raw, n);

    uint8_t g = pot_update(&pot_g, g_med);
    uint8_t b = pot_update(&pot_b, b_med);

    if (g != led_g || b != led_b) {
        led_g = g;
        led_b = b;
        rgb_set(led_r_pct, led_g, led_b);
    }
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    process(hadc, adc_readings, half_len);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    process(hadc, adc_readings + half_len, half_len);
}

int main()
{
    HAL_Init();
    SystemClock_Config();
    generated_MPU_Config();
    MX_TIM4_Init();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();

    HAL_TIM_PWM_Start(PWM_TIMER, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(PWM_TIMER, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(PWM_TIMER, TIM_CHANNEL_3);
    rgb_set(led_r_pct, led_g, led_b);

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_readings, adc_buf_len);

    while (1)
    {
        __WFI();
    }
}
