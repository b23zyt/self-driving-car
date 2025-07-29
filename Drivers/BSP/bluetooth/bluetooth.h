#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "stdio.h"
#include "sys.h"

/* UART�շ������С */
#define UART2_RX_BUF_SIZE            128
#define UART2_TX_BUF_SIZE            64


void bt_init(uint32_t bound);            /* ���ڳ�ʼ������ */
void bt_send(char* data, uint8_t size);
void bt_transmit(char* format, ...);
#endif


