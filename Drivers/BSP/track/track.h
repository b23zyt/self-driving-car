#ifndef __TRACK_H__
#define __TRACK_H__
#include "sys.h"

#define FALSE   0
#define TRUE    1

void track_init(void);
uint8_t get_left_track();
uint8_t get_right_track();

#endif