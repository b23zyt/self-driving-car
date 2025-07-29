#include "voice.h"


//initialize A0 A1 and A2
void voice_init(){
    GPIO_InitTypeDef gpio_initstruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    gpio_initstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    gpio_initstruct.Mode = GPIO_MODE_INPUT;
    gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_initstruct);
}