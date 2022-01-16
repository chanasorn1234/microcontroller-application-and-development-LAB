/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint32_t adc_val = 0;
volatile uint32_t adc_avg_8 = 0;
volatile uint32_t adc_avg_16 = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int average_8();
int average_16();
void displayHEX();
void voltage();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADC_Start(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  while(HAL_ADC_PollForConversion(&hadc1, 100) != HAL_OK){}
	  adc_val = HAL_ADC_GetValue(&hadc1);
	  adc_avg_8 = average_8(adc_val);
	  adc_avg_16 = average_16(adc_val);
	  if(adc_avg_8 <= 819){
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_SET);

	  }
	  else if(adc_avg_8 > 819 && adc_avg_8 <= 1638 ){

		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_SET);

	  }
	  else if(adc_avg_8 > 1638 && adc_avg_8 <= 2457 ){

		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	 	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
	 	  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_SET);

	 	  }
	  else if(adc_avg_8 > 2457 && adc_avg_8 <= 3276 ){

	  	  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_RESET);
	  	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	  	  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
	  	  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_SET);

	  	 	  }
	  else{
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, GPIO_PIN_RESET);

	  }
	  displayHEX(adc_avg_8);
	  //HAL_Delay(250);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int average_8(int x){
	static int samples[8];
	static int i = 0;
	static int total = 0;

	total += x - samples[i];
	samples[i] = x;

	i = (i == 7 ? 0 : i+1);
	return total>>3;
}

int average_16(int x){
	static int samples[16];
	static int i = 0;
	static int total = 0;

	total += x - samples[i];
	samples[i] = x;

	i = (i == 15 ? 0 : i+1);
	return total>>4;
}
void displayHEX(uint32_t input){

	char hex[10];
	int l;
	char result[10];
	int range;
	int k = 0;
	char ch1[] = "ADC1_CH10 ";
	sprintf(&hex, "%X", input);
	//printf("%s\n",hex);
    l = strlen(hex);
	result[0] = '0';
	result[1] = 'x';
	range = 8 - l;
	for(int i =2;i<range+2;i++){
        result[i] = '0';
	}
	for(int j = 10 - l;j<11;j++){
	    result[j] = hex[k];
	    k += 1;
	}
	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
	HAL_UART_Transmit(&huart3, (uint8_t*)ch1,strlen(ch1),1000);
	//HAL_Delay(500);

	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
    HAL_UART_Transmit(&huart3, (uint8_t*)result,strlen(result),1000);
    //HAL_Delay(500);
    voltage(input);
	//printf("%s",result);
	//return 0;

}

void voltage(uint32_t input){
	double voltmax = 3.3;
	double analogmax = 4095;
	double result2 = (input * voltmax) / analogmax;
	char st[10] = {0};
	char ch1[] = "V(in) = ";
	char ch2[] = "\r\n";
	char ch3[] = "V";
	char ch4 = ' ';
	result2 = round(result2 * 100.0)/100.0;
	sprintf(st,"%.2f",result2);
	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
	HAL_UART_Transmit(&huart3, (uint8_t*)&ch4,1,1000);
	//HAL_Delay(500);

	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
	HAL_UART_Transmit(&huart3, (uint8_t*)ch1,strlen(ch1),1000);
	//HAL_Delay(500);

	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
	HAL_UART_Transmit(&huart3, (uint8_t*)st,strlen(st),1000);
	//HAL_Delay(500);

	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
	HAL_UART_Transmit(&huart3, (uint8_t*)&ch4,1,1000);
	//HAL_Delay(500);

	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
	HAL_UART_Transmit(&huart3, (uint8_t*)ch3,strlen(ch3),1000);
	//HAL_Delay(500);

	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
	HAL_UART_Transmit(&huart3, (uint8_t*)ch2,strlen(ch2),1000);
	HAL_Delay(250);




}
/* USER CODE END 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
