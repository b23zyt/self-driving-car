#include "sys.h"
#include "bluetooth.h"
#include "string.h"
#include <stdarg.h>
#include "motor.h"

UART_HandleTypeDef uart2_handle;                                            /* UART1��� */

uint8_t uart2_rx_buf[UART2_RX_BUF_SIZE];                                    /* UART1���ջ����� */
uint16_t uart2_rx_len = 0;                                                  /* UART1�����ַ����� */

/**
 * @brief       ����1��ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @retval      ��
 */
 void bt_init(uint32_t baudrate)
{
    /*UART1 ��ʼ������*/
    uart2_handle.Instance = USART2;                                         /* USART1 */
    uart2_handle.Init.BaudRate = baudrate;                                  /* ������ */
    uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* �ֳ�Ϊ8λ���ݸ�ʽ */
    uart2_handle.Init.StopBits = UART_STOPBITS_1;                           /* һ��ֹͣλ */
    uart2_handle.Init.Parity = UART_PARITY_NONE;                            /* ����żУ��λ */
    uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* ��Ӳ������ */
    uart2_handle.Init.Mode = UART_MODE_TX_RX;                               /* �շ�ģʽ */
    HAL_UART_Init(&uart2_handle);                                           /* HAL_UART_Init()��ʹ��UART1 */
}


/**
 * @brief       UART1���ջ��������
 * @param       ��
 * @retval      ��
 */
void uart2_rx_clear(void)
{
    memset(uart2_rx_buf, 0, sizeof(uart2_rx_buf));                          /* ��ս��ջ����� */
    uart2_rx_len = 0;                                                       /* ���ռ��������� */
}

/**
 * @brief       ����1�жϷ�����
 * @note        �ڴ�ʹ�ý����жϼ������жϣ�ʵ�ֲ����������շ�
 * @param       ��
 * @retval      ��
 */
void USART2_IRQHandler(void)
{
    uint8_t receive_data = 0;   
    if(__HAL_UART_GET_FLAG(&uart2_handle, UART_FLAG_RXNE) != RESET){        /* ��ȡ����RXNE��־λ�Ƿ���λ */
        if(uart2_rx_len >= sizeof(uart2_rx_buf))                            /* ������յ��ַ������ڽ��ջ�������С�� */
            uart2_rx_len = 0;                                               /* �򽫽��ռ��������� */
        HAL_UART_Receive(&uart2_handle, &receive_data, 1, 1000);            /* ����һ���ַ� */
        uart2_rx_buf[uart2_rx_len++] = receive_data;                        /* �����յ����ַ������ڽ��ջ����� */
    }

    if (__HAL_UART_GET_FLAG(&uart2_handle, UART_FLAG_IDLE) != RESET)        /* ��ȡ���տ����жϱ�־λ�Ƿ���λ */
    {
        printf("bluetooth receive: %s\r\n", uart2_rx_buf);                               /* �����յ������ݴ�ӡ���� */
        
        //check the incoming message from UART2
        if(strcmp((char *)uart2_rx_buf, "M1") == 0)
            motor_forward();
        else if(strcmp((char *)uart2_rx_buf, "M2") == 0)
            motor_backward();
        else if(strcmp((char *)uart2_rx_buf, "M3") == 0)
            motor_left();
        else if(strcmp((char *)uart2_rx_buf, "M4") == 0)
            motor_right();
        else
            motor_stop();
        
        uart2_rx_clear();
        __HAL_UART_CLEAR_IDLEFLAG(&uart2_handle);                           /* ���UART���߿����ж� */
    }
}

//fix size transmit
void bt_send(char* data, uint8_t size){
    HAL_UART_Transmit(&uart2_handle, (uint8_t *)data, size, 100);
    
}

void bt_transmit(char* format, ...){
    uint8_t send_buf[128] = {0};
    va_list arg;
    va_start(arg, format);
    vsprintf((char *)send_buf, format, arg);
    va_end(arg);
    HAL_UART_Transmit(&uart2_handle, send_buf, sizeof(send_buf), 100);
}
