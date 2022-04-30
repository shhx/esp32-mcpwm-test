#include <Arduino.h>
#include "driver/mcpwm.h"

#if defined(__cplusplus)
extern "C"
{
#endif

typedef struct {
    uint32_t gpio;
    mcpwm_unit_t unit;
    mcpwm_timer_t timer;
    mcpwm_io_signals_t io_signal;
} motor_hw_config_t;

typedef struct {
    uint32_t frequency;
    float duty;
    uint32_t delay_us;
    motor_hw_config_t hw_config;
} motor_t;

void init_motor(motor_t *motor, int gpio, mcpwm_unit_t unit, mcpwm_timer_t timer, uint32_t freq, float duty);
void motor_set_delay(motor_t *motor, uint32_t delay_us, const motor_t *ref_motor);
void motor_set_freq(motor_t *motor, uint32_t freq);
void motor_set_duty(motor_t *motor, float duty);

#if defined(__cplusplus)
}
#endif