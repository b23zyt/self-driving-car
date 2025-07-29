#include "sys.h"
#include "delay.h"
#include "led.h"
#include "uart1.h"
#include "motor.h"
#include "speed.h"
#include "oled.h"
#include "esp8266.h"
#include <string.h>
#include "follow.h"
#include "hcsr04.h"
#include "sg90.h"
#include "track.h"
#include "voice.h"


char recv_data[ESP8266_RX_BUF_SIZE];
double middle, left, right = 0;

void manual_mode(){
    esp8266_receive_data(recv_data);
        if(strstr(recv_data, "M1") != NULL)
            motor_forward();
        else if(strstr(recv_data, "M2") != NULL)
            motor_backward();
        else if(strstr(recv_data, "M3") != NULL)
            motor_left();
        else if(strstr(recv_data, "M4") != NULL)
            motor_right();
        else 
            motor_stop();
        delay_ms(10);
}

void escape_mode(){
    middle = get_hcsr04_distance();
    if(middle > 35)
        motor_forward();
    else if(middle < 15) //move backward if too close
        motor_backward();
    //otherwise detect the distance of left and right to decide which direction to turn
    else{
        motor_stop();
        //mesure left distance then return to the middle
        sg90_left();
        delay_ms(500);
        left = get_hcsr04_distance();
        sg90_middle();
        delay_ms(500);
        
        //measure right distanc
        sg90_right();
        delay_ms(500);
        right = get_hcsr04_distance();
        
        //turn left or right based on the measurements 
        if(left < right)
            motor_right();
        else if(left >= right)
            motor_left();
        
        delay_ms(200);
        motor_stop();
        sg90_middle();
    }
    delay_ms(100);
}

void tracing_mode(){
    if(get_left_track() == TRUE && get_right_track() == TRUE)
        motor_forward();
    else if(get_left_track() == TRUE && get_right_track() == FALSE)
        motor_right();
    else if(get_left_track() == FALSE && get_right_track() == TRUE)
        motor_left();
    else
        motor_stop();
}

void following_mode(){
    if(get_left_follow() == TRUE && get_right_follow() == TRUE)
        motor_forward();
    else if(get_left_follow() == TRUE && get_right_follow() == FALSE)
        motor_left();
    else if(get_left_follow() == FALSE & get_right_follow() == TRUE)
        motor_right();
    else
        motor_stop();
}

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    led_init();
    led1_on();
    led2_off();
    uart1_init(115200);
    esp8266_init(115200);
    motor_init();
    //bt_init(115200);
    speed_init();
    printf("Hello World!\r\n");
    oled_init();
    oled_fill(0x00);
    follow_init();
    hcsr04_init();
    sg90_init();
    track_init();
    voice_init();
    
    printf("Hello World!\r\n");
    
    manual_mode();
    
    while(1)
    {     
        //manual_mode();
        if(TRACE_PIN == GPIO_PIN_RESET && FOLLOW_PIN == GPIO_PIN_SET && ESCAPE_PIN == GPIO_PIN_SET){
            tracing_mode();
            //esp8266_rx_clear();
        }else if(TRACE_PIN == GPIO_PIN_SET && FOLLOW_PIN == GPIO_PIN_RESET && ESCAPE_PIN == GPIO_PIN_SET){
            following_mode();
            //esp8266_rx_clear();
        }else if(TRACE_PIN == GPIO_PIN_SET && FOLLOW_PIN == GPIO_PIN_SET && ESCAPE_PIN == GPIO_PIN_RESET){
            escape_mode();
            //esp8266_rx_clear();
        }else if(TRACE_PIN == GPIO_PIN_RESET && FOLLOW_PIN == GPIO_PIN_RESET && ESCAPE_PIN == GPIO_PIN_RESET){
            manual_mode(); //default mode
        }
    }
}