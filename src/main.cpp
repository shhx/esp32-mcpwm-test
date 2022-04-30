#include <Arduino.h>

#include "driver/mcpwm.h"
#include "motor.h"

#define GPIO_PWM0A_OUT 32   
#define GPIO_PWM0B_OUT 33   
#define GPIO_PWM1A_OUT 25   
#define GPIO_PWM1B_OUT 26   
#define GPIO_PWM2A_OUT 17   
#define GPIO_PWM2B_OUT 2 

#define MAX_MOTORS  3

motor_t motors[MAX_MOTORS];
uint32_t gpio_list[MAX_MOTORS] = {GPIO_PWM0A_OUT, GPIO_PWM1A_OUT, GPIO_PWM2A_OUT};
uint32_t freq_list[MAX_MOTORS] = {100, 100, 100};

void setup()
{
    Serial.begin(115200);
    Serial.println("Start");
    for (uint8_t i = 0; i < MAX_MOTORS; i++)
    {
        init_motor(&motors[i], gpio_list[i], MCPWM_UNIT_0, (mcpwm_timer_t)i, freq_list[i], 20);
    }
    
    motor_set_delay(&motors[1], 150, &motors[0]);
}

void loop()
{
    
}