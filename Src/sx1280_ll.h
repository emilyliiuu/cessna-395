/*
 * sx1280_ll.h
 *
 *  Created on: Sep 25, 2025
 *      Author: emilyliiuu
 */

#ifndef SRC_SX1280_SX1280_LL_H_
#define SRC_SX1280_SX1280_LL_H_

#include <stdint.h>
#include "stm32l0xx_hal.h"

typedef struct{
	uint16_t pin;
	GPIO_TypeDef *port;
} io_type;

typedef struct{
	io_type nss;  		//gpio
	io_type dio1;		//gpio
	io_type reset;
	void *hspi;
} module;

void init(module *m);

void reset(module *m);

void write_byte(module *m, uint8_t data);

uint8_t read_byte(module *m);

void wait_busy();

void set_nss(module *m, int value);

void write_command(module *m, uint8_t opcode, uint8_t *buffer, uint16_t size);

void read_command(module *m, uint8_t opcode, uint8_t *buffer, uint16_t size);

void read_command_signed(module *m, uint8_t opcode, int8_t *buffer, uint16_t size);

void write_registers(module *m, uint16_t addr, uint8_t *buffer, uint16_t size);

void read_registers(module *m, uint16_t addr, uint8_t *buffer, uint16_t size);

uint8_t read_register(module *m, uint16_t addr);

void write_buffer(module *m, uint8_t offset, uint8_t *buffer, uint8_t size);

void read_buffer(module *m, uint8_t offset, uint8_t *buffer, uint8_t size);



#endif /* SRC_SX1280_SX1280_LL_H_ */
