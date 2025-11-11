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
#include "sx1280.h"
#include "sx1280_ll.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
sx1280 sx;
volatile bool dio1_flag = false;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void print_hex(uint8_t *data, uint8_t len) {
    char buf[5];
    for (uint8_t i = 0; i < len; i++) {
        sprintf(buf, "%02X ", data[i]);
        HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
}

void print_bin_16(uint16_t data) {
    char buf[17];  // 16 bits + null terminator

    for (int8_t bit = 15; bit >= 0; bit--) {
        buf[15 - bit] = (data & ((uint16_t)1 << bit)) ? '1' : '0';
    }
    buf[16] = '\0';  // null terminate

    HAL_UART_Transmit(&huart2, (uint8_t*)buf, 16, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
}

void print_bin_8(uint8_t data) {
    char buf[9];  // 16 bits + null terminator

    for (int8_t bit = 7; bit >= 0; bit--) {
        buf[7 - bit] = (data & ((uint16_t)1 << bit)) ? '1' : '0';
    }
    buf[8] = '\0';  // null terminate

    HAL_UART_Transmit(&huart2, (uint8_t*)buf, 8, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, (uint8_t*)"\r\n", 2, HAL_MAX_DELAY);
}




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
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  module m = { .hspi = &hspi1,
  			 .nss = { GPIO_PIN_4, GPIOA },
  			 .reset = {GPIO_PIN_3, GPIOB },
  			 .dio1 = { GPIO_PIN_9, GPIOA } };


  memset(&sx, 0, sizeof(sx1280));

  sx.m = &m;

  uint8_t buffer[0xFF];
  memset( &buffer, 0x00, 0xFF );


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {


//		TEST 1: BLINKING LED (pass)
//		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
//		  HAL_Delay(100);


//		TEST 2: TURN LED ON WITH BUTTON (pass)
//		  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)){
//			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
//		  }
//		  else{
//			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//		  }


//		TEST 3: SPI COMMS (pass)
//		init(&m);
//		reset(&m);
//
//		set_nss(&m, 0);
//		write_byte(&m, 0xC0);
//		uint8_t resp = read_byte(&m);
//
//		set_nss(&m, 1);
//
//		if (resp) {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
//		}
//		while (1);


//		TEST 4: READ REGISTER (FIRMWARE VERSION) (pass)
//		reset(&m);
//		set_nss(&m, 1);
//
//		uint8_t fw_version[2] = {0};
//		ReadRegister(&sx, 0x0153, fw_version, 2);
//
//		print_hex(fw_version, sizeof(fw_version));
//
//		uint16_t fw = (fw_version[1] << 8) | fw_version[0];
//
//
//		if(fw == 0xB7A9 || fw == 0xB5A9){
//		    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
//		} else {
//		    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//		}
//		while(1);

//		TEST 5: WRITE AND READ REGISTER (pass)
//		reset(&m);
//
//		uint8_t write[1] = {0x1A};
//		uint8_t read[1] = {0};
//
//		write_registers(sx.m, 0x0900, write, 1);
//
//		print_hex(write, sizeof(write));
//
//		read_registers(sx.m, 0x0900, read, 1);
//
//		print_hex(read, sizeof(read));
//
//		if((read[0] == 0x1A) && (read[0] == write[0])){
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
//		} else {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//		}
//		while(1);

//		UART PRINTING
//		uint8_t Test[] = "Hello World !!!\r\n"; //Data to send
//		HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);// Sending in normal mode
//		HAL_Delay(1000);

//		TEST 6: READ AND WRITE BUFFER (pass)
//		reset(&m);
//
//		uint8_t write[2] = {0x1A, 0x1B};
//		uint8_t read[2] = {0};
//
//		write_buffer(sx.m, TX_BASEADDR, write, 2);
//
//		print_hex(write, sizeof(write));
//
//		read_buffer(sx.m, TX_BASEADDR, read, 2);
//
//		print_hex(read, sizeof(read));
//
//		if((read[0] == 0x1A) && (read[1] == 0x1B)){
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
//		} else {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//		}
//		HAL_Delay(2000);
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//
//		uint8_t write1[3] = {0x1A, 0x1B, 0x1C};
//		uint8_t read1[3] = {0};
//
//		write_buffer(sx.m, TX_BASEADDR, write1, 3);
//
//		print_hex(write1, sizeof(write1));
//
//		read_buffer(sx.m, TX_BASEADDR, read1, 3);
//
//		print_hex(read1, sizeof(read1));
//
//		while(1);

//		TEST 7: SETSTANDBY (pass)
//		init(&m);
//		reset(&m);
//		SetSleep(&sx, SLEEP_RAM_RETEN);
//		HAL_Delay(2000);
//		set_nss(&m, 0);
//		set_nss(&m, 1);
//		wait_busy();
//		config(&sx);
//
//		uint16_t IRQMASK = RXDONE | RXTXTIMEOUT;
//		uint16_t DIO1MASK = RXDONE | RXTXTIMEOUT;
//		SetDioIrqParams(&sx, IRQMASK, DIO1MASK, 0x00, 0x00); 	//txdone and rxtxtimeout to dio1
////		SetStandby(&sx, STDBY_XOSC);
//		uint8_t s1 = GetStatus(&sx);
//		print_bin_8(s1);
//		uint16_t irq1 = GetIrqStatus(&sx);
//		print_bin_16(irq1);
//		ClearIrqStatus(&sx, CLEAR_ALL);
////		uint16_t irq = GetIrqStatus(&sx);
////		print_bin_16(irq);
//
////		SetStandby(&sx, STDBY_XOSC); //works
//		SetRx(&sx);	//works
//
////		SetTxParams(&sx);
////		SetTx(&sx); //works
////		uint8_t packet[1] = {0x01};
////		send_payload(&sx, packet, 1);
//		uint8_t status = GetStatus(&sx);
//		print_bin_8(status);
//
//		while(!dio1_flag){}; 	//wait for txdone or timeout
//
//		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)==GPIO_PIN_SET){
//			uint8_t Test[] = "dio1 true";
//			HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);// Sending in normal mode
//		}
//		dio1_flag = false;
//
//		HAL_Delay(2000);
//
//		uint16_t irq11 = GetIrqStatus(&sx);
//		print_bin_16(irq11);
//
//
//		while(1);

//  }
//		FINAL TEST TEST
//		bool transmit = false;
//		if (transmit){	//transmitter
//			init(&m);
//			reset(&m);
//			config(&sx);
//
//			SetTxParams(&sx);
//			uint16_t IRQMASK = TXDONE | RXTXTIMEOUT;
//			uint16_t DIO1MASK = TXDONE | RXTXTIMEOUT;
//			SetDioIrqParams(&sx, IRQMASK, DIO1MASK, 0x00, 0x00); 	//txdone and rxtxtimeout to dio1
////			SetTxContinuousWave(&sx);
//			uint8_t status = GetStatus(&sx);
//			print_bin_8(status);
//
//
//
//			while(1){
//				if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)){
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
//
//					//write consecutive bytes
////					uint8_t packet[1] = {0x00};
////					for(int i = 0; i<16; i++){
////						packet[0] += 1;
////						print_hex(packet, 1);
////						send_payload(&sx, packet, 1);
////					}
//
//					//write long packet
//					uint8_t packet[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
//					send_payload(&sx, packet, 8);
//
//
//					while(!dio1_flag){}; 	//wait for txdone or timeout
//					dio1_flag = false;
//					uint8_t status = GetStatus(&sx);
//					print_bin_8(status);
//					ClearIrqStatus(&sx, CLEAR_ALL);
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//					HAL_Delay(5000);
//
//				}
//				else{
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//				}
//
////				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
////				while(!dio1_flag){}; 	//wait for txdone or timeout
////				dio1_flag = false;
////				uint8_t status = GetStatus(&sx);
////				print_bin_8(status);
////				ClearIrqStatus(&sx, CLEAR_ALL);
////				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//
//			}
//
//		}
//
//		else{		//receiver
//			init(&m);
//			reset(&m);
//			SetStandby(&sx, STDBY_XOSC);
//			config(&sx);
//
//
//			uint16_t IRQMASK = RXDONE | CRCERROR;
//			uint16_t DIO1MASK = RXDONE | CRCERROR;
//			SetDioIrqParams(&sx, IRQMASK, DIO1MASK, 0x00, 0x00); //rxdone and crcerror to dio1
//			ClearIrqStatus(&sx, CLEAR_ALL);
//			SetRx(&sx);
//
//			while(1){
//				uint8_t status1 = GetStatus(&sx);
////				print_bin_8(status1);
//				while(!dio1_flag){
////					uint8_t rssi = GetRssiInst(&sx);
////					char buf[64];
////					int rssiDbm = -(rssi / 2);
////					int n = sprintf(buf, "RSSIinst = %d dBm\r\n", rssiDbm);
////					if (rssiDbm >-60){
////						HAL_UART_Transmit(&huart2, (uint8_t*)buf, n, HAL_MAX_DELAY);
////					}
//				}
//
////				if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)==GPIO_PIN_SET){
////					uint8_t Test[] = "dio1 true";
////					HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);
////				}
//
//				dio1_flag = false;
//
//
//				uint8_t packet[8];
//				uint8_t size[1] = {0x08};
//				get_payload(&sx, packet, size, 0x08);
//				print_hex(packet, 8);
//
////				uint8_t offset[1];
////
////				GetRxBufferStatus(&sx, size, offset);
////				print_hex(offset[0], 1);
//
//				uint16_t packetstatus = GetPacketStatus(&sx);
////				print_bin_16(packetstatus);
//
//				uint8_t rssi1 = (packetstatus>>8)&0x00FF;
//				char buf1[64];
//				int rssiDbm1 = -(rssi1 / 2);
//				int n1 = sprintf(buf1, "RSSIinst = %d dBm\r\n", rssiDbm1);
//				HAL_UART_Transmit(&huart2, (uint8_t*)buf1, n1, HAL_MAX_DELAY);
//
//				uint16_t irq = GetIrqStatus(&sx);
////				print_bin_16(irq);
//				if(irq & CRCERROR){
//					ClearIrqStatus(&sx, CLEAR_ALL);
//					continue; //bad packet
//				}
////				uint16_t packetstatus = GetPacketStatus(&sx);
//				//maybe do some checks
////				if(packetstatus==0x0000){
////					continue;
////				}
//				ClearIrqStatus(&sx, CLEAR_ALL);
//				HAL_UART_Transmit(&huart2,packet,sizeof(packet),10);
//				if (packet[0] == 0xAB){
//
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
//					HAL_Delay(10);
//				}
//				else{
//					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
//				}
//
//			}
//
//
//		}

//		TRANSCIEVER SWITCHING
		bool transmit = true;
		if (transmit){	//transmitter
			init(&m);
			reset(&m);
			config(&sx);

			SetTxParams(&sx);
			uint16_t IRQMASK = TXDONE | RXTXTIMEOUT;
			uint16_t DIO1MASK = TXDONE | RXTXTIMEOUT;
			SetDioIrqParams(&sx, IRQMASK, DIO1MASK, 0x00, 0x00); 	//txdone and rxtxtimeout to dio1
//			SetTxContinuousWave(&sx);
			uint8_t status = GetStatus(&sx);
			print_bin_8(status);



			while(1){
				if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)){

					uint8_t packet[8] = {0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
					send_payload(&sx, packet, 8);
					while(!dio1_flag){};
					dio1_flag = false;


					uint8_t status = GetStatus(&sx);
					print_bin_8(status);
					ClearIrqStatus(&sx, CLEAR_ALL);
					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
					HAL_Delay(500);

				}
				else{

					uint8_t switch_packet[8] = {0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
					send_payload(&sx, switch_packet, 8);
					while(!dio1_flag){};
					dio1_flag = false;
					//switch to rx
					uint8_t Test[] = "switching to rx\r\n";
					HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);
					IRQMASK = RXDONE | CRCERROR;
					DIO1MASK = RXDONE | CRCERROR;
					SetDioIrqParams(&sx, IRQMASK, DIO1MASK, 0x00, 0x00); //rxdone and crcerror to dio1
					ClearIrqStatus(&sx, CLEAR_ALL);
					SetRx(&sx);

					uint8_t status1 = GetStatus(&sx);
					print_bin_8(status1);
					while(!dio1_flag){}

					dio1_flag = false;
					uint8_t received[8];
					uint8_t size[1] = {0x08};
					get_payload(&sx, received, size, 0x08);
					uint8_t text[] = "packet received";
					HAL_UART_Transmit(&huart2,text,sizeof(text),10);
					print_hex(received, 8);

					ClearIrqStatus(&sx, CLEAR_ALL);



					if (received[7] == 0xFF){			//switch back to tx
						uint8_t Test[] = "switching to tx\r\n";
						HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
						IRQMASK = TXDONE | RXTXTIMEOUT;
						DIO1MASK = TXDONE | RXTXTIMEOUT;
						SetDioIrqParams(&sx, IRQMASK, DIO1MASK, 0x00, 0x00); 	//txdone and rxtxtimeout to dio1
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
						HAL_Delay(500);
					}
					//otherwise keep listening

				}
			}

		}

		else{		//receiver
			init(&m);
			reset(&m);
			SetStandby(&sx, STDBY_XOSC);
			config(&sx);
			SetTxParams(&sx);


			uint16_t IRQMASK = RXDONE | CRCERROR;
			uint16_t DIO1MASK = RXDONE | CRCERROR;
			SetDioIrqParams(&sx, IRQMASK, DIO1MASK, 0x00, 0x00); //rxdone and crcerror to dio1
			ClearIrqStatus(&sx, CLEAR_ALL);
			SetRx(&sx);

			while(1){

				while(!dio1_flag){}

				dio1_flag = false;

				uint8_t read[8];
				uint8_t size[1] = {0x08};
				get_payload(&sx, read, size, 0x08);
				print_hex(read, 8);

//				uint16_t packetstatus = GetPacketStatus(&sx);

				ClearIrqStatus(&sx, CLEAR_ALL);

				HAL_UART_Transmit(&huart2,read,sizeof(read),10);

				if (read[7] == 0xFF){		//set tx single
					uint8_t Test[] = "switching to tx\r\n";
					HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);
					uint16_t IRQMASK = TXDONE | RXTXTIMEOUT;
					uint16_t DIO1MASK = TXDONE | RXTXTIMEOUT;
					SetDioIrqParams(&sx, IRQMASK, DIO1MASK, 0x00, 0x00); 	//txdone and rxtxtimeout to dio1
					uint8_t packet[8] = {0xAB, 0xCD, 0xEF, 0x01, 0x00, 0x00, 0x00, 0xFF};
					HAL_Delay(100);
					send_payload(&sx, packet, 8);
					while(!dio1_flag){}
					dio1_flag = false;
					uint8_t status = GetStatus(&sx);
					uint8_t circuit_status = status & 0b11100000;
					print_bin_8(status);
					uint16_t irqstat = GetIrqStatus(&sx);
					print_bin_16(irqstat);
					if(circuit_status == 0x40){
						uint8_t Test[] = "transmit success, switching to rx\r\n";
						HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);
						uint16_t IRQMASK = RXDONE | CRCERROR;
						uint16_t DIO1MASK = RXDONE | CRCERROR;
						SetDioIrqParams(&sx, IRQMASK, DIO1MASK, 0x00, 0x00); //rxdone and crcerror to dio1
						ClearIrqStatus(&sx, CLEAR_ALL);
						SetRx(&sx);
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
						HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

					}
					else{
						uint8_t Test[] = "not in standby, bad transmit\r\n";
						HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);
						HAL_Delay(10000);
					}


				}
				//otherwise continue listening

			}


		}



	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 19200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_9) {
	  uint8_t Test[] = "interrupt triggered\r\n"; //Data to send
	  HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);// Sending in normal mode
//	  uint16_t irq = GetIrqStatus(&sx);
//	  ClearIrqStatus(&sx, irq);

	  dio1_flag = true;
  } else {
      __NOP();
  }
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
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
