#include "timer.h"

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef tim2_handler;

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }else if(htim->Instance == TIM2){
        __HAL_RCC_TIM2_CLK_ENABLE();
    }
}

void TIM3_Init(void) {
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 7200-1;
    htim3.Init.Period = 10000-1;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim3);
    HAL_TIM_Base_Start_IT(&htim3);
}

void TIM2_Init(void){
    tim2_handler.Instance = TIM2;
    tim2_handler.Init.Prescaler = 71;
    tim2_handler.Init.Period = 65535;
    tim2_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim2_handler.Init.AutoReloadPreload = TIM_OUTPUTNSTATE_DISABLE;
    
    HAL_TIM_Base_Init(&tim2_handler);
}