#ifndef __FOLLOW_H__
#define __FOLLOW_H__
#include "sys.h"

#define FALSE   0
#define TRUE    1

void follow_init(void);
uint8_t get_left_follow();
uint8_t get_right_follow();

#endif