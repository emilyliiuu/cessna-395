#include "sx1280_ll.h"
#include "sx1280.h"
#include "stm32l0xx_hal.h"
#include <string.h>
#include <stdint.h>


static uint8_t halTxBuffer[0xFF] = {0x00};
static uint8_t halRxBuffer[0xFF] = {0xFF};


void init(module *m)
{
    set_nss(m, 1);
    HAL_GPIO_WritePin(m->reset.port, m->reset.pin, GPIO_PIN_SET);
}

void reset(module *m){
	HAL_Delay(20);
    HAL_GPIO_WritePin(m->reset.port, m->reset.pin, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(m->reset.port, m->reset.pin, GPIO_PIN_SET);
    HAL_Delay(20);
}

void wait_busy(){
	while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET) { ; }
}

void set_nss(module *m, int value){
	if(value==1){
		HAL_GPIO_WritePin(GPIOA, m->nss.pin, GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(GPIOA, m->nss.pin, GPIO_PIN_RESET);
	}
}

void write_byte(module *m, uint8_t data)
{
    HAL_SPI_Transmit(m->hspi, &data, 1, HAL_MAX_DELAY);
}

uint8_t read_byte(module *m)
{
	set_nss(m, 0);
    uint8_t txbyte = 0x00;
    uint8_t rxbyte = 0x00;

    HAL_SPI_TransmitReceive(m->hspi, &txbyte, &rxbyte, 1, HAL_MAX_DELAY);
    set_nss(m, 1);
    return rxbyte;
}

void write_command(module *m, uint8_t opcode, uint8_t *buffer, uint16_t size )
{
    uint16_t halSize  = size + 1;
    wait_busy();

    set_nss(m, 0);

    halTxBuffer[0] = opcode;
    memcpy( halTxBuffer + 1, ( uint8_t * )buffer, size * sizeof( uint8_t ) );

    HAL_SPI_Transmit(m->hspi, halTxBuffer, halSize, HAL_MAX_DELAY );

    set_nss(m, 1);

    if(opcode != SET_SLEEP){
    	wait_busy();
    }

}

void read_command(module *m, uint8_t opcode, uint8_t *buffer, uint16_t size)
{
    uint16_t halSize = 2 + size;
    halTxBuffer[0] = opcode;
    halTxBuffer[1] = 0x00;
    for( uint16_t index = 0; index < size; index++ )
    {
        halTxBuffer[2+index] = 0x00;
    }

    wait_busy();

    set_nss(m, 0);

    HAL_SPI_TransmitReceive(m->hspi, halTxBuffer, halRxBuffer, halSize, HAL_MAX_DELAY );

    if(opcode==GET_STATUS){
    	memcpy( buffer,  (uint8_t *)halRxBuffer, size );
    }
    else{
    	memcpy( buffer,  (uint8_t *)(halRxBuffer+2), size );
    }
    set_nss(m, 1);

    wait_busy();
}

void read_command_signed(module *m, uint8_t opcode, int8_t *buffer, uint16_t size)
{
    uint16_t halSize = 2 + size;
    halTxBuffer[0] = opcode;
    halTxBuffer[1] = 0x00;
    for( uint16_t index = 0; index < size; index++ )
    {
        halTxBuffer[2+index] = 0x00;
    }

    wait_busy();

    set_nss(m, 0);

    HAL_SPI_TransmitReceive(m->hspi, halTxBuffer, halRxBuffer, halSize, HAL_MAX_DELAY );

    if(opcode==GET_STATUS){
    	memcpy( buffer,  ( int8_t * )halRxBuffer, size );
    }
    else{
    	memcpy( buffer,  ( int8_t * )(halRxBuffer+2), size );
    }
    set_nss(m, 1);

    wait_busy();
}


//void write_buffer(module *m, uint8_t *data, uint8_t len) {
//	set_nss(m, 0);
//    for (uint8_t i = 0; i < len; i++) {
//        HAL_SPI_Transmit(m->hspi, &data[i], 1, 1000);
//        while(HAL_SPI_GetState(m->hspi) != HAL_SPI_STATE_READY) { ; }
//    }
//    set_nss(m, 1);
//}
//
//void read_buffer(module *m, uint8_t *data, uint8_t len) {
//	set_nss(m, 0);
//    uint8_t dummy = 0x00;
//    for (uint8_t i = 0; i < len; i++) {
//        HAL_SPI_TransmitReceive(m->hspi, &dummy, &data[i], 1, 1000);
//        while(HAL_SPI_GetState(m->hspi) != HAL_SPI_STATE_READY) { ; }
//    }
//    set_nss(m, 1);
//}

void write_registers(module *m, uint16_t addr, uint8_t *buffer, uint16_t size )
{
    uint16_t halSize = size + 3;
    halTxBuffer[0] = WRITEREG;
    halTxBuffer[1] = ( addr & 0xFF00 ) >> 8;
    halTxBuffer[2] = addr & 0x00FF;

    memcpy( halTxBuffer + 3, buffer, size );
    wait_busy();
    set_nss(m, 0);
    HAL_SPI_Transmit(m->hspi, halTxBuffer, halSize, HAL_MAX_DELAY );
    set_nss(m, 1);
    wait_busy();
}

void read_registers(module *m, uint16_t addr, uint8_t *buffer, uint16_t size )
{
    uint16_t halSize = 4 + size;
    halTxBuffer[0] = READREG;
    halTxBuffer[1] = ( addr & 0xFF00 ) >> 8;
    halTxBuffer[2] = addr & 0x00FF;
    halTxBuffer[3] = 0x00;
    for( uint16_t index = 0; index < size; index++ )
    {
        halTxBuffer[4+index] = 0x00;
    }

    wait_busy();
    set_nss(m, 0);
    HAL_SPI_TransmitReceive(m->hspi, halTxBuffer, halRxBuffer, halSize, HAL_MAX_DELAY );
    memcpy( buffer, halRxBuffer + 4, size );
    set_nss(m, 1);
    wait_busy();
}

uint8_t read_register(module *m, uint16_t addr )
{
    uint8_t data;
    read_registers(m, addr, &data, 1 );
    return data;
}

void write_buffer(module *m, uint8_t offset, uint8_t *buffer, uint8_t size )
{
	uint16_t halSize = size + 2;
	halTxBuffer[0] = WRITE_BUF;
	halTxBuffer[1] = offset;
	memcpy( halTxBuffer + 2, buffer, size );

	wait_busy();
	set_nss(m, 0);
	HAL_SPI_Transmit(m->hspi, halTxBuffer, halSize, HAL_MAX_DELAY );
	set_nss(m, 1);
	wait_busy();
}

void read_buffer(module *m, uint8_t offset, uint8_t *buffer, uint8_t size )
{
    uint16_t halSize = size + 3;
    halTxBuffer[0] = READ_BUF;
    halTxBuffer[1] = offset;
    halTxBuffer[2] = 0x00;
    for( uint16_t index = 0; index < size; index++ )
    {
        halTxBuffer[3+index] = 0x00;
    }

    wait_busy();
	set_nss(m, 0);
	HAL_SPI_TransmitReceive(m->hspi, halTxBuffer, halRxBuffer, halSize, HAL_MAX_DELAY );
	memcpy( buffer, halRxBuffer + 3, size );
	set_nss(m, 1);
	wait_busy();
}


//uint8_t get_dio1(module *m)
//{
//    return HAL_GPIO_ReadPin(m->dio1.port, m->dio1.pin) == GPIO_PIN_SET ? 1 : 0;
//}
