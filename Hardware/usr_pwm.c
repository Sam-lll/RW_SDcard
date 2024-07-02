#include "usr_pwm.h"
#include "stdafx.h"
#include "main.h"

extern TIM_HandleTypeDef htim2;

// void Delay(uint32_t Delay)
// {
//     // zssLogDEBUG("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
//   uint32_t tickstart = HAL_GetTick();
// //   zssLogDEBUG("<%s  %d> %s(tickstart: %d)", __FILE__, __LINE__, __FUNCTION__, tickstart);
//   uint32_t wait = Delay;

//   /* Add a freq to guarantee minimum wait */
//   if (wait < HAL_MAX_DELAY)
//   {
//     wait += 1;
//   }

//   while ((HAL_GetTick() - tickstart) < wait)
//   {
//     // zssLogDEBUG("<%s  %d> %s(HAL_GetTick(): %d)", __FILE__, __LINE__, __FUNCTION__, HAL_GetTick());
//   }
// //   zssLogDEBUG("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
// }

void pwm_msc_init(TIM_HandleTypeDef* htim){
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    HAL_TIM_PWM_Start(htim,TIM_CHANNEL_2);//开启PWM
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}

void pwm_msc_unlock_Step1(TIM_HandleTypeDef* htim){
    // Implementation goes here
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,2000);//BB响两声
    // __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,1003);//BB响两声
	HAL_Delay(5000 * 100);//延时4S
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}

void pwm_msc_unlock_Step2(TIM_HandleTypeDef* htim){
    // Implementation goes here
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,1000);//BBB响三声
    // __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,2003);//BB响两声
	HAL_Delay(3000 * 100);
	
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}

void pwm_msc_start(TIM_HandleTypeDef* htim){    
    // Implementation goes here
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,1049);//起飞
    // __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,1100);//起飞
	HAL_Delay(3000 * 100);
                        
    zssLogINFO("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}