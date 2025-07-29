#include "hcsr04.h"
#include "timer.h"

extern TIM_HandleTypeDef tim2_handler;

//1 period = 1us, arr = 65535
//void tim2_init(void){
//    tim2_handler.Instance = TIM2;
//    tim2_handler.Init.Prescaler = 71;
//    tim2_handler.Init.Period = 65535;
//    tim2_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
//    tim2_handler.Init.AutoReloadPreload = TIM_OUTPUTNSTATE_DISABLE;
//    
//    HAL_TIM_Base_Init(&tim2_handler);
//}

//MSP (MCU Support Package) including GPIO NVIC EXTI
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim){
//    if(htim->Instance == TIM2){
//        __HAL_RCC_TIM2_CLK_ENABLE();
//    }
//}

void tim2_start(void){
    HAL_TIM_Base_Start(&tim2_handler);
}

void tim2_stop(void){
    HAL_TIM_Base_Stop(&tim2_handler);
}

uint16_t get_tim2_cnt(void){
    return __HAL_TIM_GetCounter(&tim2_handler);
}

void set_tim2_cnt(uint16_t val){
    __HAL_TIM_SetCounter(&tim2_handler, val);
}

void hcsr04_gpio_init(void){
    GPIO_InitTypeDef gpio_initstruct;
    
    TRIG_GPIO_CLK_ENABLE();
    ECHO_GPIO_CLK_ENABLE();
    
    gpio_initstruct.Pin = TRIG_PIN;
    gpio_initstruct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initstruct.Pull = GPIO_NOPULL;
    gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TRIG_PORT, &gpio_initstruct);
    
    gpio_initstruct.Pin = ECHO_PIN;
    gpio_initstruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(ECHO_PORT, &gpio_initstruct);
}

void hcsr04_init(void){
    TIM2_Init();
    hcsr04_gpio_init();
}



float get_hcsr04_distance(void){
    uint16_t total_cnt = 0;
    //1. send 10us high sig to TRIG
    TRIG_HIGH();
    delay_us(15);
    TRIG_LOW();
    //2. READ ECHO, from LOW to HIGH -> start to send wave
    
    while(ECHO_STATUS() == GPIO_PIN_RESET);
    tim2_start();
    set_tim2_cnt(0);
    
    //3. ECHO from HIGH to LOW -> wave comes back, count the time
    while(ECHO_STATUS() == GPIO_PIN_SET);
    tim2_stop();
    
    //4. count the timer that ECHO is HIGH
    total_cnt = get_tim2_cnt();
    
    //5. Distance = spped * time /2 
    //343m/s * 0.000001s/us * 100cm/m * 2  = 0.01715
    float distance = total_cnt * 0.01715; 
    return distance;
}