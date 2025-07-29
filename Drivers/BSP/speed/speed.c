#include "speed.h"
#include "oled.h"
#include "delay.h"
#include "esp8266.h"
#include "timer.h"

//basic logic of speed detection system: 
/*
speed module: if there is a baffle between the sensor, it outputs high level and the led is off. If nothing is between the sensor, it outputs a low level and the led is on

the diameter of the wheel for the car is 6.5 cm, therefore the parimenter is 3.14 x 6.5 cm
The pallet has 20 compartments, therefore, for each cycle of the wheel, there are 20 rising and falling edges
Each falling edge represents that the car has travelled 3.14 x 6.5 cm / 20 = 1cm

To detect the speed: we can trigger an interrupt whenever there's a falling edge of that pin.
Then use a timer to count the number of interrupts triggered within 1s, and the speed is #interrupts/1s
*/

uint16_t speed_cnt = 0;
char speed_str[24] = {0};
//TIM_HandleTypeDef timer_handle = {0};
extern TIM_HandleTypeDef htim3;

void speed_gpio_init(){
    GPIO_InitTypeDef gpio_initstruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    gpio_initstruct.Pin = GPIO_PIN_12;
    gpio_initstruct.Mode = GPIO_MODE_IT_FALLING;
    gpio_initstruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &gpio_initstruct);
    
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

//void speed_timer_init(uint16_t arr, uint16_t psc){
//    timer_handle.Instance = TIM3;
//    timer_handle.Init.Prescaler = psc;
//    timer_handle.Init.Period = arr;
//    timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
//    timer_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//    HAL_TIM_Base_Init(&timer_handle);
//    HAL_TIM_Base_Start_IT(&timer_handle);
//}


void speed_init(){
    speed_gpio_init();
    //speed_timer_init(10000-1, 7200-1); //10^4 * 7200 / (72*10^6) = 1s
    TIM3_Init();
}


void EXTI15_10_IRQHandler(){
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    uint32_t last_tick = 0;
    if(GPIO_Pin == GPIO_PIN_12){
        uint32_t current_tick = HAL_GetTick();
        delay_us(500);
        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET){
            //things to do when interrupt happened
            speed_cnt++;
            //printf("speed count: %d\r\n", speed_cnt);
        }
    }
}

//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim){
//    if(htim->Instance == TIM3){
//        __HAL_RCC_TIM3_CLK_ENABLE();
//        HAL_NVIC_SetPriority(TIM3_IRQn,2, 1);
//        HAL_NVIC_EnableIRQ(TIM3_IRQn);
//    }
//}

void TIM3_IRQHandler(){
    HAL_TIM_IRQHandler(&htim3);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    //get the number of interrupts in 1s
    if(htim->Instance == TIM3){
        //printf("Speed count: %d\r\n", speed_cnt);
        memset(speed_str, 0, sizeof(speed_str)); //empty speed_str first
        //display the speed on the OLED screen;
        sprintf(speed_str, "Speed: %2dcm/s", speed_cnt);
        oled_display_string(10, 2, speed_str, 16);
        //send the speed to the phone over bluetooth
        //bt_send(speed_str, 24);
        //bt_send("\r\n", 10);
        esp8266_send_data(speed_str);
        speed_cnt = 0;
    }
}


