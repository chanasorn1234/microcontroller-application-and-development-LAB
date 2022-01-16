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
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TXBUFLEN (50)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t adc_val[8];
//const int adcChannelcount = sizeof (adc_val) / sizeof (adc_val[0]);
volatile int adcConversionComplete = 0;
char *channel[] = {"ADC1_CH3 ","ADC1_CH4 ","ADC1_CH5 ","ADC1_CH6 ","ADC1_CH9 ","ADC1_CH10 ","ADC1_CH12 ","ADC1_CH13 "};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  //HAL_ADC_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1,adc_val, 8);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
	  //char txBuf[TXBUFLEN];
	  while(adcConversionComplete == 0){}
	  adcConversionComplete = 0;
	  //sprintf(txBuf,"%d %d %d %d %d %d %d %d\r\n",adc_val[0],adc_val[1],adc_val[2],adc_val[3],adc_val[4],adc_val[5],adc_val[6],adc_val[7]);
	  //HAL_UART_Transmit(&huart3, (uint8_t*)txBuf,strlen(txBuf),1000);
	  for(int i=0;i<8;i++){
		  HAL_UART_Transmit(&huart3, (uint8_t*)channel[i],strlen(channel[i]),1000);
		  displayHEX(adc_val[i]);
		  if(i == 3){
			  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_RESET);
		  }
		  if(i == 7){
			  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
		  }
	  }
	  HAL_Delay(1000);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV6;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){
	//char ch1 = 'a' ;
	//HAL_UART_Transmit(&huart3, (uint8_t*)&ch1,1,1000);
	//static int cha = 0;
	//if(cha == 4){
	//HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
		//cha = 0;
	//}
	//cha++;
	//HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
	/*char ch3[] = "ADC1_CH3 ";
	char ch4[] = "ADC1_CH4 ";
	char ch5[] = "ADC1_CH5 ";
	char ch6[] = "ADC1_CH6 ";
	char ch9[] = "ADC1_CH9 ";
	char ch10[] = "ADC1_CH10 ";
	char ch12[] = "ADC1_CH12 ";
	char ch13[] = "ADC1_CH13 ";
	if(hadc == ADC_CHANNEL_3){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch3,strlen(ch3),1000);
	}
	if(hadc == ADC_CHANNEL_4){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch4,strlen(ch4),1000);
	}
	if(hadc == ADC_CHANNEL_5){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch5,strlen(ch5),1000);
	}
	if(hadc == ADC_CHANNEL_6){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch6,strlen(ch6),1000);
	}
	if(hadc == ADC_CHANNEL_9){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch9,strlen(ch9),1000);
	}
	if(hadc == ADC_CHANNEL_10){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch10,strlen(ch10),1000);
	}
	if(hadc == ADC_CHANNEL_12){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch12,strlen(ch12),1000);
	}
	if(hadc == ADC_CHANNEL_13){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch13,strlen(ch13),1000);
	}
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_RESET);*/
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	/*static int cha = 0;
	if(cha == 4){
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);
		cha = 0;
	}
	cha++;*/
	//HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_RESET);
	//char ch1 = 'b' ;
	//HAL_UART_Transmit(&huart3, (uint8_t*)&ch1,1,1000);
	adcConversionComplete = 1;
	/*char ch3[] = "ADC1_CH3 ";
	char ch4[] = "ADC1_CH4 ";
	char ch5[] = "ADC1_CH5 ";
	char ch6[] = "ADC1_CH6 ";
	char ch9[] = "ADC1_CH9 ";
	char ch10[] = "ADC1_CH10 ";
	char ch12[] = "ADC1_CH12 ";
	char ch13[] = "ADC1_CH13 ";
	if(hadc == ADC_CHANNEL_3){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch3,strlen(ch3),1000);
	}
	if(hadc == ADC_CHANNEL_4){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch4,strlen(ch4),1000);
	}
	if(hadc == ADC_CHANNEL_5){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch5,strlen(ch5),1000);
	}
	if(hadc == ADC_CHANNEL_6){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch6,strlen(ch6),1000);
	}
	if(hadc == ADC_CHANNEL_9){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch9,strlen(ch9),1000);
	}
	if(hadc == ADC_CHANNEL_10){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch10,strlen(ch10),1000);
	}
	if(hadc == ADC_CHANNEL_12){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch12,strlen(ch12),1000);
	}
	if(hadc == ADC_CHANNEL_13){
		while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*)ch13,strlen(ch13),1000);
	}
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, GPIO_PIN_SET);*/

}
void displayHEX(uint32_t input){

	char hex[10];
	int l;
	char result[10];
	int range;
	int k = 0;
	//char ch1[] = "ADC1_CH10 ";
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
	//while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
	//HAL_UART_Transmit(&huart3, (uint8_t*)ch1,strlen(ch1),1000);
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
