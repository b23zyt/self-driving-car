#include "sg90.h"

TIM_HandleTypeDef pwm_handler = {0};

//psc = 7199, (frequency = 10000Hz, period = 10^-4s, arr =199, counts for 20ms 
void tim3_init(void){
    TIM_OC_InitTypeDef pwm_config = {0};
    
    pwm_handler.Instance = TIM3;
    pwm_handler.Init.Prescaler = 7199;
    pwm_handler.Init.Period = 199;
    pwm_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_PWM_Init(&pwm_handler);
    
    pwm_config.OCMode = TIM_OCMODE_PWM1;
    pwm_config.Pulse = 200/2; //value of CCR, initially set to arr/2
    pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
    HAL_TIM_PWM_ConfigChannel(&pwm_handler, &pwm_config, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&pwm_handler, TIM_CHANNEL_1);
}

//MSP
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim){
    if(htim->Instance == TIM3){
        
        //PA6
        GPIO_InitTypeDef gpio_initstruct;
        
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_TIM3_CLK_ENABLE();
        
        gpio_initstruct.Pin = GPIO_PIN_6;
        gpio_initstruct.Mode = GPIO_MODE_AF_PP;
        gpio_initstruct.Pull = GPIO_PULLUP;
        gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &gpio_initstruct);
    }
}

//change CCR value
void tim3_compare_set(uint16_t val){
    __HAL_TIM_SET_COMPARE(&pwm_handler, TIM_CHANNEL_1, val);
}

//sg90
void sg90_init(void){
    tim3_init();
}

void set_sg90_angle(uint16_t angle){
    uint16_t CCRx = 5 + angle/9;
    tim3_compare_set(CCRx);
}

void sg90_left(){
    set_sg90_angle(180);
}

void sg90_middle(){
    set_sg90_angle(90);
}

void sg90_right(){
    set_sg90_angle(0);
}
