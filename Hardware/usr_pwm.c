#include "usr_pwm.h"
#include "stdafx.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;

void pwm_msc_init(TIM_HandleTypeDef* htim){
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    HAL_TIM_PWM_Start(htim,TIM_CHANNEL_2);                  //enable PWM
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, 900);        //music, then bb, then b..... long volume
    USART_SendFormatString("please insert vcc line\r\n");
    HAL_Delay(3000 * 100);
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}

void pwm_msc_start(TIM_HandleTypeDef* htim){    
    // Implementation goes here
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,1000);        //motor begin rotate
	HAL_Delay(3000 * 100);
                        
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}