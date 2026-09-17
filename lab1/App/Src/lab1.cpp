#include "lab1.h"
#include "gpio.h"

static constexpr unsigned MIN_TIME_S = 1;
static constexpr unsigned MAX_TIME_S = 5;
static constexpr unsigned SUBTICK_MS = 10;

static volatile State    state       = State::Stopped;
static volatile unsigned step_time_s = MIN_TIME_S;
static unsigned          highlighted_pin = 0;

static unsigned next_pin(unsigned i) { return i + 1 == LED_COUNT ? 0 : i + 1; }
static unsigned prev_pin(unsigned i) { return i == 0 ? LED_COUNT - 1 : i - 1; }

// interrupt handling for joystick buttons
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    switch (pin) {
    case JOYA_Pin:   if (step_time_s < MAX_TIME_S) step_time_s++; break;  // ^
    case JOYD_Pin:   if (step_time_s > MIN_TIME_S) step_time_s--; break;  // v
    case JOYB_Pin:   state = State::RightRun; break;                      // >
    case JOYC_Pin:   state = State::LeftRun;  break;                      // <
    case JOYCTR_Pin: state = State::Stopped;  break;                      // center
    default: break;
    }
}

// turns off every led except "n"
static void highlight_pin(unsigned n)
{
    for (unsigned i = 0; i < LED_COUNT; i++) {
        HAL_GPIO_WritePin(
            LEDS[i].port,
            LEDS[i].pin,
            i == n ? GPIO_PIN_SET : GPIO_PIN_RESET
        );
    }
}

int main()
{
    HAL_Init();
    SystemClock_Config();
    generated_MPU_Config();
    MX_GPIO_Init();

    highlight_pin(highlighted_pin);
    uint32_t last_step  = HAL_GetTick();
    State    prev_state = state;

    while (1)
    {
        const State s = state;

        if (s != prev_state) {
            prev_state = s;
            last_step  = HAL_GetTick();
        }
        // GetTick() - last_step would fail after 49 day of run due to unsigned logic
        // not a problem I think =)
        if (s != State::Stopped &&
            HAL_GetTick() - last_step >= step_time_s * 1000u)
        {
            last_step = HAL_GetTick();
            highlighted_pin = (s == State::LeftRun) ? next_pin(highlighted_pin)
                                                    : prev_pin(highlighted_pin);
            highlight_pin(highlighted_pin);
        }

        HAL_Delay(SUBTICK_MS);
    }
}
