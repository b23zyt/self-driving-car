#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "sys.h"

#define ESP8266_RX_BUF_SIZE  128
#define ESP8266_TX_BUF_SIZE  64

#define ESP8266_EOK        0
#define ESP8266_ERROR      1
#define ESP8266_ETIMEOUT   2
#define ESP8266_EINVAL     3

#define ESP8266_STA_MODE        1
#define ESP8266_AP_MODE         2
#define ESP8266_STA_AP_MODE     3

#define ESP8266_SINGLE_CONECTION    0
#define ESP8266_MULTI_CONNECTION    1

#define WIFI_NAME       "955"
#define WIFI_PASSWORD   "jackdashabi123"

#define TCP_SERVER_IP       "192.168.50.231"
#define TCP_SERVER_PORT     "8080"

void esp8266_init(uint32_t baudrate);
uint16_t esp8266_receive_data(char* recv);
void esp8266_send_data(char* fmt, ...);
void esp8266_send_test(void);
void esp8266_rx_clear(void);
#endif


