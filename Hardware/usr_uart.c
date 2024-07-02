#include <stm32f1xx_hal.h>
#include "usr_uart.h"
#include "main.h"
#include <string.h>
#include "stdafx.h"
#include <assert.h>

UART_HandleTypeDef huart1;
uint8_t* strCurfile;
uint8_t RxBuffer[RxBuffer_Size];
uint8_t TxBuffer[TxBuffer_Size];
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

// Function to initialize USART communication
void USART_Init() {
    // Implementation goes here
    zssLogDEBUG("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_IN");
    // HAL_StatusTypeDef hal_stat = HAL_USART_Receive_IT(&husart1, Serial_RxPacket, sizeof(Serial_RxPacket));
    // if(HAL_OK != hal_stat)
    // {
    //     zssLogERROR("<%s  %d> %s(hal_stat: %d)", __FILE__, __LINE__, __FUNCTION__, hal_stat);
    // }
    zssLogDEBUG("<%s  %d> %s(arg: %s)", __FILE__, __LINE__, __FUNCTION__, "LOG_OUT");
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART1_UART_Init(UART_HandleTypeDef* huart)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart->Instance = USART1;
  huart->Init.BaudRate = 460800;
  huart->Init.WordLength = UART_WORDLENGTH_8B;
  huart->Init.StopBits = UART_STOPBITS_1;
  huart->Init.Parity = UART_PARITY_NONE;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(huart) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  
  /* USER CODE END USART1_Init 2 */

}


// Function to send a single character over USART
void USART_SendByte(uint8_t c) {
    // Implementation goes here
    // Function to send a single character over USART
    // HAL_StatusTypeDef hal_stat = HAL_USART_Transmit(&husart1, &c, 1, HAL_MAX_DELAY);
    // if(hal_stat != HAL_OK)
    // {
    //     zssLogERROR("<%s  %d> %s(hal_stat: %d)", __FILE__, __LINE__, __FUNCTION__, hal_stat);
    // }
    // while (HAL_USART_GetState(&husart1) == HAL_USART_STATE_RESET)
    // {
    //     /* code */
    // }
}
// Function to send a null-terminated string over USART
void USART_SendString(const char* str) {
    // Implementation goes here
    zssLogDEBUG("<%s  %d> %s(str: %s)", __FILE__, __LINE__, __FUNCTION__, str);
    // HAL_StatusTypeDef hal_stat = HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
    HAL_StatusTypeDef hal_stat = HAL_UART_Transmit_DMA(&huart1, (uint8_t*)str, strlen(str));
    if(hal_stat != HAL_OK)
    {
        zssLogERROR("<%s  %d> %s(hal_stat: %d)", __FILE__, __LINE__, __FUNCTION__, hal_stat);
    }

    HAL_UART_StateTypeDef uart_stat;
    int timeout = 1;  // Adjust this value as needed
    while ((uart_stat = HAL_UART_GetState(&huart1)) != HAL_UART_STATE_READY && timeout > 0)
    {
        /* code */
        zssLogDEBUG("<%s  %d> %s(uart_stat: %d)", __FILE__, __LINE__, __FUNCTION__, uart_stat);
        timeout--;
    }
    if (timeout <= 0) {
        zssLogERROR("<%s  %d> %s(timeout)", __FILE__, __LINE__, __FUNCTION__);
    }
    return;
}

void USART_SendFormatString(char *format, ...) {
    // Implementation goes here
    // zssLogDEBUG("<%s  %d> %s(str: %s)", __FILE__, __LINE__, __FUNCTION__, str);
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(TxBuffer, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
    // HAL_StatusTypeDef hal_stat = HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
    HAL_StatusTypeDef hal_stat = HAL_UART_Transmit_DMA(&huart1, (uint8_t*)TxBuffer, strlen(TxBuffer));
    if(hal_stat != HAL_OK)
    {
        zssLogERROR("<%s  %d> %s(hal_stat: %d)", __FILE__, __LINE__, __FUNCTION__, hal_stat);
    }

    HAL_UART_StateTypeDef uart_stat;
    int timeout = 1;  // Adjust this value as needed
    while ((uart_stat = HAL_UART_GetState(&huart1)) != HAL_UART_STATE_READY && timeout > 0)
    {
        /* code */
        zssLogDEBUG("<%s  %d> %s(uart_stat: %d)", __FILE__, __LINE__, __FUNCTION__, uart_stat);
        timeout--;
    }
    if (timeout <= 0) {
        zssLogERROR("<%s  %d> %s(timeout)", __FILE__, __LINE__, __FUNCTION__);
    }
    return;
}

void USART_SendNum(int num) {
    // Implementation goes here
    char str[32]; // Make sure the array is large enough to hold the number
    // snprintf(str, sizeof(str), "%2d", num);
    itoa(num, str, 10);
    zssLogDEBUG("<%s  %d> %s(str: %s)", __FILE__, __LINE__, __FUNCTION__, str);
    // HAL_StatusTypeDef hal_stat = HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), HAL_MAX_DELAY);
    HAL_StatusTypeDef hal_stat = HAL_UART_Transmit_DMA(&huart1, (uint8_t*)str, strlen(str));
    if(hal_stat != HAL_OK)
    {
        zssLogERROR("<%s  %d> %s(hal_stat: %d)", __FILE__, __LINE__, __FUNCTION__, hal_stat);
    }

    HAL_UART_StateTypeDef uart_stat;
    int timeout = 1;  // Adjust this value as needed
    while ((uart_stat = HAL_UART_GetState(&huart1)) != HAL_UART_STATE_READY && timeout > 0)
    {
        /* code */
        zssLogDEBUG("<%s  %d> %s(uart_stat: %d)", __FILE__, __LINE__, __FUNCTION__, uart_stat);
        timeout--;
    }
    if (timeout <= 0) {
        zssLogERROR("<%s  %d> %s(timeout)", __FILE__, __LINE__, __FUNCTION__);
    }
    return;
}
// Function to send an array of bytes over USART
void USART_SendData(const uint8_t* data, uint32_t length) {
    // Implementation goes here
    // HAL_USART_Transmit(&husart1, data, length, HAL_MAX_DELAY);
    // Implementation goes here
    
    assert(data != NULL);
    // HAL_StatusTypeDef hal_stat = HAL_UART_Transmit_DMA(&huart1, data, length);
    HAL_StatusTypeDef hal_stat = HAL_UART_Transmit(&huart1, data, length, HAL_MAX_DELAY);
    if(hal_stat != HAL_OK)
    {
        zssLogERROR("<%s  %d> %s(hal_stat: %d)", __FILE__, __LINE__, __FUNCTION__, hal_stat);
    }

    HAL_UART_StateTypeDef uart_stat;
    int timeout = 100;  // Adjust this value as needed
    while ((uart_stat = HAL_UART_GetState(&huart1)) != HAL_UART_STATE_READY && timeout > 0)
    {
        /* code */
        // zssLogDEBUG("<%s  %d> %s(uart_stat: %d)", __FILE__, __LINE__, __FUNCTION__, uart_stat);
        timeout--;
    }
    if (timeout <= 0) {
        zssLogERROR("<%s  %d> %s(timeout)", __FILE__, __LINE__, __FUNCTION__);
    }
    return;
}

// Function to receive a single character over USART
uint8_t USART_ReceiveChar() {
    uint8_t c;
    // HAL_USART_Receive(&husart1, &c, 1, HAL_MAX_DELAY);
    return c;
}

// Function to receive a null-terminated string over USART
void USART_ReceiveString(char* strbuf, uint32_t bufSize) {
    // Implementation goes here
    // HAL_USART_Receive(&husart1, strbuf, bufSize, HAL_MAX_DELAY);
}

// Function to receive an array of bytes over USART
void USART_ReceiveData(uint8_t* data, uint32_t length) {
    // Implementation goes here
    // HAL_USART_Receive(&husart1, data, length, HAL_MAX_DELAY);
}

// HAL transmit complete callback function
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	// memset(user_data, 0, strlen(user_data)); //empty the transmission data buffer
}

// HAL reception complete callback function
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
     // write some code when reception is complete
}

void UART1_IRQHandler(void)
{
    
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	// check if the callback is called by the USART2 peripheral
	if(huart->Instance == USART1){
		// take length of the RxBuffer  
		int len = strlen(RxBuffer);
		// Copy RxBuffer to TxBuffer
		strncpy(TxBuffer,RxBuffer,len);
        if(0 == strcmp(RxBuffer, "extract")){
            SetExtFlag(1);
        }
        else if(strstr(RxBuffer, ".log")){
            strCurfile = strdup(RxBuffer);
            SetlogFlag(1);
        }
        else if(0 == strcmp(RxBuffer, "unlock")){
            SetUnlockFlag(1);
        }
        else if(0 == strcmp(RxBuffer, "unlock2")){
            SetUnlockFlagStep2(1);
        }
        else if(0 == strcmp(RxBuffer, "delete")){
            ;
        }
        else if(0 == strcmp(RxBuffer, "start")){
            SetStartFlag(1);
        }
        else{
            // Transmit the TxBuffer data over uart
		    HAL_UART_Transmit(&huart1,TxBuffer,len,HAL_MAX_DELAY);
            // HAL_UART_Transmit_DMA(&huart1, TxBuffer, sizeof(TxBuffer));
        }
        
		// empty the TxBuffer and RxBuffer buffer
		memset(TxBuffer, 0, sizeof(TxBuffer));
		memset(RxBuffer, 0, sizeof(RxBuffer));
		// Initialize the DMA conversion
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, RxBuffer, RxBuffer_Size);
		__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);

	}
}
