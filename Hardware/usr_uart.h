#pragma once

#define MAX_TEXT_LENGTH 100

// Function declaration for USART initialization
void USART_Init();

void MX_USART1_UART_Init(UART_HandleTypeDef* huart);

// Function declaration to send a single character over USART
void USART_SendByte(uint8_t c);

// Function declaration to send a null-terminated string over USART
void USART_SendString(const char* str);

void USART_SendFormatString(char *format, ...);

void USART_SendNum(int num);
// Function declaration to send an array of bytes over USART
void USART_SendData(const uint8_t* data, uint32_t length);

// Function declaration to receive a single character over USART
uint8_t USART_ReceiveChar();

// Function declaration to receive a null-terminated string over USART
void USART_ReceiveString(char* strbuf, uint32_t bufSize);

// Function declaration to receive an array of bytes over USART
void USART_ReceiveData(uint8_t* data, uint32_t length);
