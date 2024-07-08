#pragma once
#include <stm32f1xx_hal.h>

#define MAX_SPEED       2160
#define MIN_SPEED       900
#define GEAR_SEGMENT    200
#define START_GEAR      1000

void pwm_msc_init(TIM_HandleTypeDef* htim);
void pwm_msc_start(TIM_HandleTypeDef* htim);
void pwm_msc_brake(TIM_HandleTypeDef* htim);
void pwm_msc_pause(TIM_HandleTypeDef* htim);
void pwm_msc_inc_speed(TIM_HandleTypeDef* htim);
void pwm_msc_dec_speed(TIM_HandleTypeDef* htim);