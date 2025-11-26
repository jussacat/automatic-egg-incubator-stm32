/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "i2c-lcd.h"
#include "string.h"
#include "stdlib.h"

#include "keyboardMatrix_def.h"
#include "keyboardMatrix.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DHT22_PORT GPIOA
#define DHT22_PIN GPIO_PIN_11

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

uint8_t hum1, hum2, tempC1, tempC2, SUM, CHECK;
uint32_t pMillis, cMillis;
volatile float temp_Celsius = 0;
float temp_Fahrenheit = 0;
float Humidity = 0;
uint8_t hum_integral, hum_decimal, tempC_integral, tempC_decimal,
		tempF_integral, tempF_decimal;
float temp_C = 0;
char string[32];

volatile uint32_t seconds = 0;

uint8_t incubationDay = 0;
char buffer[32];
uint8_t current_menu = 0; // 0: temp_high, 1: temp_low, 2: incupationDay
volatile char key;


static uint8_t buzzer_active = 0;
float temp_high = 38.0;
float temp_low = 23.5;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1) {
		// Xử lý cho TIM2
		seconds++;
		if (seconds >= 86400) {
			incubationDay++;
			seconds = 0;
		}
	}
}

void microDelay(uint16_t delay) {
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	while (__HAL_TIM_GET_COUNTER(&htim2) < delay);
}

uint8_t DHT22_Start(void) {
	uint8_t Response = 0;
	GPIO_InitTypeDef GPIO_InitStructPrivate = { 0 };
	GPIO_InitStructPrivate.Pin = DHT22_PIN;
	GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DHT22_PORT, &GPIO_InitStructPrivate);
	HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 0);
	microDelay(1300);
	HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 1);
	microDelay(30);
	GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructPrivate.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DHT22_PORT, &GPIO_InitStructPrivate);
	microDelay(40);
	if (!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN))) {
		microDelay(80);
		if ((HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)))
			Response = 1;
	}
	pMillis = HAL_GetTick();
	cMillis = HAL_GetTick();
	while ((HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)) && pMillis + 2 > cMillis) {
		cMillis = HAL_GetTick();
	}
	return Response;
}
uint8_t DHT22_Read(void) {
	uint8_t x, y;
	for (x = 0; x < 8; x++) {
		pMillis = HAL_GetTick();
		cMillis = HAL_GetTick();
		while (!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN))
				&& pMillis + 2 > cMillis) {
			cMillis = HAL_GetTick();
		}
		microDelay(40);
		if (!(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)))   // if the pin is low
			y &= ~(1 << (7 - x));
		else
			y |= (1 << (7 - x));
		pMillis = HAL_GetTick();
		cMillis = HAL_GetTick();
		while ((HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN))
				&& pMillis + 2 > cMillis) {  // wait for the pin to go low
			cMillis = HAL_GetTick();
		}
	}
	return y;
}

void Display_Menu() {
	lcd_clear();
	switch (current_menu) {
	case 0:
		lcd_put_cur(0, 0);
		sprintf(buffer, "%.2f C", temp_high);
		lcd_send_string(buffer);
		break;
	case 1:
		lcd_put_cur(0, 0);
		sprintf(buffer, "%.2f C", temp_low);
		lcd_send_string(buffer);
		break;
	case 2:
		lcd_put_cur(0, 0);
		sprintf(buffer, "%d ", incubationDay);
		lcd_send_string(buffer);
		break;
	}
}

void Menu_Mode() {
	Display_Menu();
	uint8_t exit_menu = 0;
	while (!exit_menu) {
		key = getKeyPressed();
		if (key != '\0') {

			switch (key) {
			case '*':
				lcd_clear();
				lcd_put_cur(0, 0);
				lcd_send_string("Da luu cai dat!");
				HAL_Delay(1000);
				lcd_clear();
				exit_menu = 1;
				break;


			case '4': // Lên menu
				current_menu = (current_menu + 2) % 3;
				break;
			case '6': // Xuống menu
				current_menu = (current_menu + 1) % 3;
				break;
			case '2': // Tăng
				if (current_menu == 0 && temp_high < 40.0f)
					temp_high += 0.1f;
				else if (current_menu == 1 && temp_low < 38.0f)
					temp_low += 0.1f;
				else if (current_menu == 2 && incubationDay < 32)
					incubationDay += 1;
				break;
			case '8': // Giảm
				if (current_menu == 0 && temp_high > 37.0f)
					temp_high -= 0.1f;
				else if (current_menu == 1 && temp_low > 20.0f)
					temp_low -= 0.1f;
				else if (current_menu == 2 && incubationDay > 0)
					incubationDay -= 1;
				break;
			}
			Display_Menu();
			HAL_Delay(200); // chống dội phím
		}
	}
}
void Read_DHT22_Update_LCD(void) {
	if (DHT22_Start()) {
		hum1 = DHT22_Read();
		hum2 = DHT22_Read();
		tempC1 = DHT22_Read();
		tempC2 = DHT22_Read();
		SUM = DHT22_Read();
		CHECK = hum1 + hum2 + tempC1 + tempC2;
		if (CHECK == SUM) {
			//nếu bit MSB của tempC1 = 1 thì nhiệt độ âm
			if (tempC1 > 127) {
				temp_Celsius = (float) tempC2 / 10 * (-1);
			} else {
				temp_Celsius = (float) ((tempC1 << 8) | tempC2) / 10;
			}
			//nhiệt độ đơn vị Fahrenheit
			temp_Fahrenheit = temp_Celsius * 9 / 5 + 32;
			Humidity = (float) ((hum1 << 8) | hum2) / 10; //độ ẩm


			if (temp_Celsius < 0) {
				tempC_integral = temp_Celsius * (-1);
				tempC_decimal = temp_Celsius * (-10) - tempC_integral * 10;
				sprintf(string, "-%d.%d C   ", tempC_integral, tempC_decimal);
			} else {
				tempC_integral = temp_Celsius;
				tempC_decimal = temp_Celsius * 10 - tempC_integral * 10;
				sprintf(string, "%d.%d C   ", tempC_integral, tempC_decimal);
			}
			//Hiển thị nhiệt độ dưới dạng độ C
			lcd_put_cur(1, 0);
			lcd_send_string(string);
			temp_C = tempC_integral + (tempC_decimal / 10.0f);

			//Hiển thị độ ẩm %
			lcd_put_cur(0, 0);
			hum_integral = Humidity;
			hum_decimal = Humidity * 10 - hum_integral * 10;
			sprintf(string, "%d.%d %%  ", hum_integral, hum_decimal);
			lcd_send_string(string);

			//Hiển thị nhiệt độ dưới dạng độ F
			if (temp_Fahrenheit < 0) {
				tempF_integral = temp_Fahrenheit * (-1);
				tempF_decimal = temp_Fahrenheit * (-10) - tempF_integral * 10;
				sprintf(string, "-%d.%d F   ", tempF_integral, tempF_decimal);
			} else {
				tempF_integral = temp_Fahrenheit;
				tempF_decimal = temp_Fahrenheit * 10 - tempF_integral * 10;
				sprintf(string, "%d.%d F   ", tempF_integral, tempF_decimal);
			}
			lcd_put_cur(0, 8);
			lcd_send_string(string);

		}
	}
}
//			}

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
  MX_TIM1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

	HAL_TIM_Base_Start_IT(&htim1); //ngắt counter ở timer 2 để đếm thời gian
	HAL_TIM_Base_Start(&htim2); //counter để tạo hàm microDelay
	lcd_init();
	HAL_Delay(500);
	lcd_clear();
	lcd_put_cur(0,0);
	lcd_send_string("Bat dau ap trung");
	HAL_Delay(2000);
	lcd_clear();
	keyboardInit();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	Read_DHT22_Update_LCD();
	//Khi nhiệt độ vượt quá ngưỡng đã cho thì còi buzzer sẽ kêu
		if (temp_C >= temp_high || temp_C <= temp_low) {
			if (!buzzer_active) {
				buzzer_active = 1;
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
			}
		} else {
			if (buzzer_active) {
				buzzer_active = 0;
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); // Tắt buzzer
			}
		}
	HAL_Delay(1000);
	key = getKeyPressed();
	if (key != '\0') {
		HAL_Delay(200);
		if (key == '#') {
			Menu_Mode();
		}
	}
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 7200-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, R1_Pin|R2_Pin|R3_Pin|R4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : C4_Pin C3_Pin C2_Pin C1_Pin */
  GPIO_InitStruct.Pin = C4_Pin|C3_Pin|C2_Pin|C1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 R1_Pin R2_Pin R3_Pin
                           R4_Pin PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|R1_Pin|R2_Pin|R3_Pin
                          |R4_Pin|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
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
	while (1) {
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
