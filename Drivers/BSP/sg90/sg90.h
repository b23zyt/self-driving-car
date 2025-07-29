#ifndef __SG90_H__
#define __SG90_H__

#include "sys.h"

void sg90_init(void);
void set_sg90_angle(uint16_t angle);
void sg90_left();
void sg90_middle();
void sg90_right();
#endif