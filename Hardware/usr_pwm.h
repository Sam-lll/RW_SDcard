#pragma once
#include <stm32f1xx_hal.h>

void pwm_msc_init(TIM_HandleTypeDef* htim);
void pwm_msc_unlock_Step1(TIM_HandleTypeDef* htim);
void pwm_msc_unlock_Step2(TIM_HandleTypeDef* htim);
void pwm_msc_start(TIM_HandleTypeDef* htim);