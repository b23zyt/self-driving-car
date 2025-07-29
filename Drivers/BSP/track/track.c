#include "track.h"
#include "delay.h"
#include "led.h"

//PB13 and PB14

void track_init(void){

    GPIO_InitTypeDef gpio_initstruct;
    
    //clock enable
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    gpio_initstruct.Pin = GPIO_PIN_7 | GPIO_PIN_4;
    gpio_initstruct.Mode = GPIO_MODE_INPUT;
    gpio_initstruct.Pull = GPIO_NOPULL;
    gpio_initstruct.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(GPIOB, &gpio_initstruct);
    
    //set priority (PA0 -> EXTI0, preemption level, subpriority level)
//    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
//    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

//right is B4 and left is B7
uint8_t get_left_track(){
    //return reset if the sensor detects the white (TRUE)
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_RESET) return TRUE;
    else return FALSE;
}

uint8_t get_right_track(){
    //return reset if the sensor detects the white
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4) == GPIO_PIN_RESET) return TRUE;
    else return FALSE;
}

//Cannot use interrrupt in this project because it need to detect continuous data (not once)
//void EXTI0_IRQHandler(void){
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
//}

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//    delay_ms(20);
//    if(GPIO_Pin == GPIO_PIN_0){
//        if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET){
//            led1_toggle();
//        }
//    }
//}