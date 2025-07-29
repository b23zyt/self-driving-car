#ifndef __VOICE_H__
#define __VOICE_H__

#include <sys.h>

#define TRACE_PIN       HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)
#define ESCAPE_PIN      HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1)
#define FOLLOW_PIN      HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)

void voice_init();

#endif