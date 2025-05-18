#include "main.h"
#include "LoRa.h"
#include "LoRa_i.h"
#include "LoRa_ii.h"
#include "LoRa_iii.h"
#include "cmsis_os.h"
#include "string.h"

LoRa myLoRa;
LoRa_i myLoRa_i;
LoRa_ii myLoRa_ii;
LoRa_iii myLoRa_iii;

uint8_t LoRa_stat = 0;
uint8_t LoRa_stat_i = 0;
uint8_t LoRa_stat_ii = 0;
uint8_t LoRa_stat_iii = 0;

uint8_t Packet_size = 0;
uint8_t Packet_size_i = 0;
uint8_t Packet_size_ii = 0;
uint8_t Packet_size_iii = 0;

uint8_t RxBuffer[20];
uint8_t RxBuffer_i[20];
uint8_t RxBuffer_ii[20];
uint8_t RxBuffer_iii[20];

char* ack = "SMLoRaGTW902";

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;
SPI_HandleTypeDef hspi4;
SPI_HandleTypeDef hspi6;

UART_HandleTypeDef huart3;

osThreadId Task1Handle;
osThreadId Task2Handle;
osThreadId Task3Handle;
osThreadId Task4Handle;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI3_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_SPI4_Init(void);
static void MX_SPI6_Init(void);
void Task1_init(void const * argument);
void Task2_init(void const * argument);
void Task3_init(void const * argument);
void Task4_init(void const * argument);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_USART3_UART_Init();
  MX_SPI4_Init();
  MX_SPI6_Init();

  myLoRa = newLoRa();

  myLoRa.CS_port         = NSS_GPIO_Port;
  myLoRa.CS_pin          = NSS_Pin;
  myLoRa.reset_port      = RST_GPIO_Port;
  myLoRa.reset_pin       = RST_Pin;
  myLoRa.DIO0_port       = DIO0_0_GPIO_Port;
  myLoRa.DIO0_pin        = DIO0_0_Pin;
  myLoRa.hSPIx           = &hspi1;
  myLoRa.frequency             = 868;             
  myLoRa.spredingFactor        = SF_9;            
  myLoRa.bandWidth             = BW_250KHz;       
  myLoRa.crcRate               = CR_4_8;          
  myLoRa.power                 = POWER_17db;      
  myLoRa.overCurrentProtection = 130;             
  myLoRa.preamble              = 10;              

  if(LoRa_init(&myLoRa) == LORA_OK){
  	  LoRa_stat = 1;
  }

  myLoRa_i = newLoRa_i();

  myLoRa_i.CS_port         = NSS_1_GPIO_Port;
  myLoRa_i.CS_pin          = NSS_1_Pin;
  myLoRa_i.reset_port      = RST_1_GPIO_Port;
  myLoRa_i.reset_pin       = RST_1_Pin;
  myLoRa_i.DIO0_port       = DIO0_1_GPIO_Port;
  myLoRa_i.DIO0_pin        = DIO0_1_Pin;
  myLoRa_i.hSPIx           = &hspi3;
  myLoRa_i.frequency             = 915;            
  myLoRa_i.spredingFactor        = SF_9;            
  myLoRa_i.bandWidth             = BW_250KHz;       
  myLoRa_i.crcRate               = CR_4_8;          
  myLoRa_i.power                 = POWER_17db;     
  myLoRa_i.overCurrentProtection = 130;             
  myLoRa_i.preamble              = 10;             

  if(LoRa_init_i(&myLoRa_i) == LORA_OK){
	  LoRa_stat_i = 1;
  }

  myLoRa_ii = newLoRa_ii();

  myLoRa_ii.CS_port         = NSS_2_GPIO_Port;
  myLoRa_ii.CS_pin          = NSS_2_Pin;
  myLoRa_ii.reset_port      = RST_2_GPIO_Port;
  myLoRa_ii.reset_pin       = RST_2_Pin;
  myLoRa_ii.DIO0_port       = DIO0_2_GPIO_Port;
  myLoRa_ii.DIO0_pin        = DIO0_2_Pin;
  myLoRa_ii.hSPIx           = &hspi4;
  myLoRa_ii.frequency             = 433;             
  myLoRa_ii.spredingFactor        = SF_9;            
  myLoRa_ii.bandWidth             = BW_250KHz;       
  myLoRa_ii.crcRate               = CR_4_8;          
  myLoRa_ii.power                 = POWER_17db;      
  myLoRa_ii.overCurrentProtection = 130;             
  myLoRa_ii.preamble              = 10;              

  if(LoRa_init_ii(&myLoRa_ii) == LORA_OK){
  	  LoRa_stat_ii = 1;
  }

  myLoRa_iii = newLoRa_iii();

  myLoRa_iii.CS_port         = NSS_3_GPIO_Port;
  myLoRa_iii.CS_pin          = NSS_3_Pin;
  myLoRa_iii.reset_port      = RST_3_GPIO_Port;
  myLoRa_iii.reset_pin       = RST_3_Pin;
  myLoRa_iii.DIO0_port       = DIO0_3_GPIO_Port;
  myLoRa_iii.DIO0_pin        = DIO0_3_Pin;
  myLoRa_iii.hSPIx           = &hspi6;
  myLoRa_iii.frequency             = 902;             
  myLoRa_iii.spredingFactor        = SF_9;            
  myLoRa_iii.bandWidth             = BW_250KHz;       
  myLoRa_iii.crcRate               = CR_4_8;          
  myLoRa_iii.power                 = POWER_17db;      
  myLoRa_iii.overCurrentProtection = 130;             
  myLoRa_iii.preamble              = 10;              

  if(LoRa_init_iii(&myLoRa_iii) == LORA_OK){
	  LoRa_stat_iii = 1;
  }

  LoRa_startReceiving(&myLoRa);
  HAL_Delay(50);
  LoRa_startReceiving_i(&myLoRa_i);
  HAL_Delay(50);
  LoRa_startReceiving_ii(&myLoRa_ii);
  HAL_Delay(50);
  LoRa_startReceiving_iii(&myLoRa_iii);


  osThreadDef(Task1, Task1_init, osPriorityNormal, 0, 128);
  Task1Handle = osThreadCreate(osThread(Task1), NULL);


  osThreadDef(Task2, Task2_init, osPriorityNormal, 0, 128);
  Task2Handle = osThreadCreate(osThread(Task2), NULL);


  osThreadDef(Task3, Task3_init, osPriorityNormal, 0, 128);
  Task3Handle = osThreadCreate(osThread(Task3), NULL);


  osThreadDef(Task4, Task4_init, osPriorityNormal, 0, 128);
  Task4Handle = osThreadCreate(osThread(Task4), NULL);


  osKernelStart();


  while (1)
  {

  }

}


void Task1_init(void const * argument)
{
  for(;;)
  {
	  Packet_size = LoRa_receive(&myLoRa, RxBuffer, 20);
	  osDelay(400);
  }
}


void Task2_init(void const * argument)
{
  for(;;)
  {
	  Packet_size_i = LoRa_receive_i(&myLoRa_i, RxBuffer_i, 20);
	  osDelay(400);
  }
}


void Task3_init(void const * argument)
{
  for(;;)
  {
	  Packet_size_ii = LoRa_receive_ii(&myLoRa_ii, RxBuffer_ii, 20);
	  osDelay(400);
  }
}


void Task4_init(void const * argument)
{
  for(;;)
  {
	  Packet_size_iii = LoRa_receive_iii(&myLoRa_iii, RxBuffer_iii, 20);
	  osDelay(400);
  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI4_Init(void)
{

  /* USER CODE BEGIN SPI4_Init 0 */

  /* USER CODE END SPI4_Init 0 */

  /* USER CODE BEGIN SPI4_Init 1 */

  /* USER CODE END SPI4_Init 1 */
  /* SPI4 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI4_Init 2 */

  /* USER CODE END SPI4_Init 2 */

}

/**
  * @brief SPI6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI6_Init(void)
{

  /* USER CODE BEGIN SPI6_Init 0 */

  /* USER CODE END SPI6_Init 0 */

  /* USER CODE BEGIN SPI6_Init 1 */

  /* USER CODE END SPI6_Init 1 */
  /* SPI6 parameter configuration*/
  hspi6.Instance = SPI6;
  hspi6.Init.Mode = SPI_MODE_MASTER;
  hspi6.Init.Direction = SPI_DIRECTION_2LINES;
  hspi6.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi6.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi6.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi6.Init.NSS = SPI_NSS_SOFT;
  hspi6.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi6.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi6.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi6.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi6.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI6_Init 2 */

  /* USER CODE END SPI6_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, NSS_2_Pin|RST_2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, NSS_1_Pin|RST_1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, NSS_Pin|RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED3_Pin|LED2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, RST_3_Pin|NSS_3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : NSS_2_Pin RST_2_Pin */
  GPIO_InitStruct.Pin = NSS_2_Pin|RST_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : User_Btn_Pin */
  GPIO_InitStruct.Pin = User_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(User_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO0_2_Pin */
  GPIO_InitStruct.Pin = DIO0_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO0_2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : NSS_1_Pin RST_1_Pin */
  GPIO_InitStruct.Pin = NSS_1_Pin|RST_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : NSS_Pin RST_Pin */
  GPIO_InitStruct.Pin = NSS_Pin|RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO0_0_Pin */
  GPIO_InitStruct.Pin = DIO0_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO0_0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED3_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED3_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO0_1_Pin */
  GPIO_InitStruct.Pin = DIO0_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO0_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RST_3_Pin NSS_3_Pin */
  GPIO_InitStruct.Pin = RST_3_Pin|NSS_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO0_3_Pin */
  GPIO_InitStruct.Pin = DIO0_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO0_3_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}





void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
