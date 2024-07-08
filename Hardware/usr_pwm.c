#include "usr_pwm.h"
#include "stdafx.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;

void pwm_msc_init(TIM_HandleTypeDef* htim){
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    HAL_TIM_PWM_Start(htim,TIM_CHANNEL_2);                                              //enable PWM
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, MIN_SPEED);                              //music, then bb, then b..... long volume
    USART_SendFormatString("please insert vcc line\r\n");
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}

void pwm_msc_start(TIM_HandleTypeDef* htim){    
    // Implementation goes here
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    uint16_t cur_speed = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_2);
    do
    {
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, cur_speed);                          //motor begin rotate
        HAL_Delay(200 * 100);
        cur_speed += 2;
    } while (cur_speed < START_GEAR);
    
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}

void pwm_msc_brake(TIM_HandleTypeDef* htim){
    // Implementation goes here
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, MIN_SPEED);                              //brake the motor
    HAL_Delay(50 * 100);

    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}

// stop the motor gradually
void pwm_msc_pause(TIM_HandleTypeDef* htim){
    // Implementation goes here
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    uint16_t cur_speed = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_2);                    //motor get current speed
    cur_speed -= GEAR_SEGMENT;

    while(cur_speed > MIN_SPEED){
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, cur_speed);                          //decrease the rotate speed of motor gradually
        HAL_Delay(200 * 100);
        cur_speed -= GEAR_SEGMENT;
    }

    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, MIN_SPEED);                              //decrease the rotate speed of motor
    HAL_Delay(50 * 100);

    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}

void pwm_msc_inc_speed(TIM_HandleTypeDef* htim){
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    uint16_t cur_speed = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_2);                    //motor get current speed
    if(cur_speed + GEAR_SEGMENT <= MAX_SPEED){
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, cur_speed + GEAR_SEGMENT);           //increase speed
    }
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}

void pwm_msc_dec_speed(TIM_HandleTypeDef* htim){
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    uint16_t cur_speed = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_2);                    //motor get current speed
    if(cur_speed - GEAR_SEGMENT >= MIN_SPEED){
        __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, cur_speed - GEAR_SEGMENT);           //decrease speed
    }
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}