#include <Arduino.h>

#include <Wire.h>
#include <Button2.h>
#include "driver/mcpwm.h"

#define BUTTON_1        35
#define BUTTON_2        0

#define GPIO_PWM0A_OUT 32   //Set GPIO 19 as PWM0A
#define GPIO_PWM0B_OUT 33   //Set GPIO 18 as PWM0B
#define GPIO_PWM1A_OUT 25   //Set GPIO 17 as PWM1A
#define GPIO_PWM1B_OUT 26   //Set GPIO 16 as PWM1B
#define GPIO_PWM2A_OUT 17   //Set GPIO 15 as PWM2A
#define GPIO_PWM2B_OUT 2   //Set GPIO 14 as PWM2B
#define PWM_FREQ  500
// typedef enum {
//   MCPWM_SELECT_SYNC_OFF, //!< SYNC is off
//   MCPWM_SELECT_SYNC_INT0, //!<Select SYNC0 as internal
//   MCPWM_SELECT_SYNC_INT1, //!<Select SYNC1 as internal
//   MCPWM_SELECT_SYNC_INT2, //!<Select SYNC2 as internal
//   MCPWM_SELECT_SYNC0, //!<Select SYNC0 as input
//   MCPWM_SELECT_SYNC1, //!<Select SYNC1 as input
//   MCPWM_SELECT_SYNC2, //!<Select SYNC2 as input
// } mcpwm_sync_signal_t;
String readString = "";
uint32_t cnt = 0;

void setup()
{
    Serial.begin(115200);
    Serial.println("Start");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO_PWM0B_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, GPIO_PWM1A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, GPIO_PWM1B_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, GPIO_PWM2A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2B, GPIO_PWM2B_OUT);
    mcpwm_config_t pwm_config;
    pwm_config.frequency = PWM_FREQ;
    pwm_config.cmpr_a = 20.0;       //duty cycle of PWMxA = 20.0%
    pwm_config.cmpr_b = 20.0;       //duty cycle of PWMxb = 20.0%
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);   //Configure PWM1A & PWM1B with above settings
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);   //Configure PWM2A & PWM2B with above settings
    mcpwm_set_timer_sync_output(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_SWSYNC_SOURCE_TEZ);
    // mcpwm_deadtime_enable(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_BYPASS_FED, 1000, 1000);   //Enable deadtime on PWM2A and PWM2B with red = (1000)*100ns on PWM2A
    // mcpwm_deadtime_enable(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_BYPASS_RED, 300, 2000);        //Enable deadtime on PWM1A and PWM1B with fed = (2000)*100ns on PWM1B
    // mcpwm_deadtime_enable(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_ACTIVE_RED_FED_FROM_PWMXA, 656, 67);  //Enable deadtime on PWM0A and PWM0B with red = (656)*100ns & fed = (67)*100ns on PWM0A and PWM0B generated from PWM0A
    uint32_t shift = 150e-6 * PWM_FREQ * 1000;
    mcpwm_sync_config_t sync_conf = {
      .sync_sig = MCPWM_SELECT_TIMER0_SYNC,
      .timer_val = shift,
      .count_direction = MCPWM_TIMER_DIRECTION_UP
    };
    mcpwm_sync_configure(MCPWM_UNIT_0, MCPWM_TIMER_1, &sync_conf);
    // mcpwm_sync_enable(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_SELECT_GPIO_SYNC0, shift);    //Load counter value with 20% of period counter of mcpwm timer 1 when sync 0 occurs
    Serial.println(shift);
}

void loop()
{
    cnt += 10;
    if(cnt == 2000) cnt = 0;
    // uint32_t shift = cnt * 1e-6 * PWM_FREQ * 1000;
    // mcpwm_sync_config_t sync_conf = {
    //   .sync_sig = MCPWM_SELECT_TIMER0_SYNC,
    //   .timer_val = shift,
    //   .count_direction = MCPWM_TIMER_DIRECTION_UP
    // };
    // // mcpwm_sync_disable(MCPWM_UNIT_0, MCPWM_TIMER_1);
    // mcpwm_sync_configure(MCPWM_UNIT_0, MCPWM_TIMER_1, &sync_conf);
    Serial.println(cnt);
    mcpwm_set_frequency(MCPWM_UNIT_0, MCPWM_TIMER_0, cnt);
    mcpwm_set_frequency(MCPWM_UNIT_0, MCPWM_TIMER_1, cnt);
    mcpwm_set_frequency(MCPWM_UNIT_0, MCPWM_TIMER_2, cnt);
    delay(100);
    // while (Serial.available()) {
    //     delay(1);  
    //     if (Serial.available() >0) {
    //     char c = Serial.read();  //gets one byte from serial buffer
    //     readString += c; //makes the string readString
    //     } 
    // }

    // if (readString.length() >0) {
    //     Serial.println(readString);  //so you can see the captured string 
    //     int n;
    //     char carray[6]; //converting string to number
    //     readString.toCharArray(carray, sizeof(carray));
    //     n = atoi(carray);
    //     Serial.println(n);
    //     uint32_t shift = n * 1e-6 * PWM_FREQ * 1000;
    //     mcpwm_sync_disable(MCPWM_UNIT_0, MCPWM_TIMER_1);
    //     mcpwm_sync_enable(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_SELECT_GPIO_SYNC0, shift);    //Load counter value with 20% of period counter of mcpwm timer 1 when sync 0 occurs
    //     Serial.println("Cambiado");
    //     //myservo.write(n); //for degees 0-180
    //     readString="";
    // } 

}