#include "motor.h"

void init_motor(motor_t *motor, int gpio, mcpwm_unit_t unit, mcpwm_timer_t timer, mcpwm_io_signals_t io_signal, uint32_t freq, float duty){
    motor->hw_config.unit = unit;
    motor->hw_config.timer = timer;
    motor->hw_config.gpio = gpio;
    motor->frequency = freq;
    motor->duty = duty;
    mcpwm_config_t pwm_config;
    pwm_config.frequency = motor->frequency;
    pwm_config.cmpr_a = motor->duty;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_gpio_init(motor->hw_config.unit, io_signal, gpio);
    mcpwm_init(motor->hw_config.unit, motor->hw_config.timer, &pwm_config);
    Serial.printf("Motor init %d Hz %.2f%% timer: %d\n", freq, duty, timer);
}

void motor_set_delay(motor_t *motor, uint32_t delay_us, const motor_t *ref_motor){
    mcpwm_set_timer_sync_output(ref_motor->hw_config.unit, ref_motor->hw_config.timer, MCPWM_SWSYNC_SOURCE_TEZ);
    motor->delay_us = delay_us;
    uint32_t shift = (float)(delay_us * 1e-6 * motor->frequency * 1000.0);
    float real_delay = shift  / (1e-6 * motor->frequency * 1000);
    mcpwm_sync_config_t sync_conf = {
      .sync_sig = (mcpwm_sync_signal_t)(ref_motor->hw_config.timer + MCPWM_SELECT_TIMER0_SYNC),
      .timer_val = shift,
      .count_direction = MCPWM_TIMER_DIRECTION_UP
    };
    Serial.println(shift);
    Serial.println(delay_us * 1e-6 * motor->frequency * 1000);
    mcpwm_sync_configure(motor->hw_config.unit, motor->hw_config.timer, &sync_conf);
    Serial.printf("Motor at %d set_delay %.2fus -> %.2f percent\n", motor->hw_config.gpio, real_delay, (float)shift/10);
}

void motor_set_freq(motor_t *motor, uint32_t freq){
    motor->frequency = freq;
    mcpwm_set_frequency(motor->hw_config.unit, motor->hw_config.timer, freq);
}

void motor_set_duty(motor_t *motor, float duty){
    motor->duty = duty;
    mcpwm_set_duty(motor->hw_config.unit, motor->hw_config.timer, MCPWM_GEN_A, duty);
}