/*
 * sx1280.h
 *
 *  Created on: Sep 30, 2025
 *      Author: 3mai1
 */

#ifndef SRC_SX1280_H_
#define SRC_SX1280_H_

#include "sx1280_ll.h"
#include <math.h>

/* ADDRESSES */
#define RX_BASEADDR  		0x00
#define TX_BASEADDR		  	0x80 //STILL NEED TO SET THIS USING COMMAND SETBUFFERBASEADDRESS()
#define PAYLOADSIZE			0x901

/* OPCODES */
#define GET_STATUS  			0xC0
#define GETRXBUFFERSTATUS		0x17
#define GET_PACKET_TYPE			0x03
#define GET_PACKET_STATUS 		0x1D
#define WRITEREG				0x18
#define READREG					0x19
#define WRITE_BUF  				0x1A
#define READ_BUF  				0x1B
#define SET_RX  				0x82
#define SET_TX  				0x83
#define SET_TX_PARAMS			0x8E
#define SET_RF_FREQUENCY  		0x86
#define SET_PACKET_TYPE  		0x8A
#define SET_MODULATION_PARAMS	0x8B
#define SET_PACKET_PARAMS 		0x8C
#define SET_DIO_IRQ_PARAMS		0x8D
#define SET_BUFFER_BASE_ADDR	0x8F
#define SET_STANDBY				0x80
#define SET_SLEEP				0x84
#define GET_IRQ_STATUS			0x15
#define CLEAR_IRQ_STATUS		0x97
#define GET_RSSI_INST			0x1F
#define TX_CONTINUOUS_WAVE		0xD2

/* PARAMETERS */
#define MAX_PACKET_LENGTH 	256
#define TX_PERIODBASE 		0x00
#define RX_PERIODBASE 		0x00
#define TX_BASECOUNT 		0x0000 //no timeout, tx mode until packet sent
#define RX_BASECOUNT 		0xFFFF //rx continuous mode
#define PACKET_TYPE_LORA	0x01
#define FREQ_2_4 			2400000000 //2.4 gHz
#define STDBY_RC			0x00
#define STDBY_XOSC			0x01
#define SLEEP_RAM_RETEN		0x01
#define POWER				0x1F
#define RAMPTIME			0xE0
#define SF7					0x70
#define SF12				0xC0
#define BW_1600				0x0A
#define CR_4_5				0x01 	//7?
#define CR_LI_4_5			0x05	//long interleaving is compatible with implicit header
#define CR_LI_4_8			0x07
#define PREAMBLE_12			0x06
#define HEADER_EXPLICIT		0x00	//variable length
#define HEADER_IMPLICIT		0x80	//fixed length
#define PAYLOAD_LENGTH 		0x08	//?
#define CRC_ON				0x20
#define IQ_NORMAL			0x40
#define TX_OUTPUT_POWER		0x1F
#define RADIO_RAMP_02_US	0x00
#define XTAL_FREQ           52000000
#define FREQ_STEP           ((double)(XTAL_FREQ/pow((double)2.0, (double)18.0)))


/* BIT MASKS */
#define TXDONE				0b0000000000000001	//bit 0
#define RXDONE				0b0000000000000010  //bit 1
#define RXTXTIMEOUT 		0b0100000000000000	//bit 14
#define CRCERROR			0b0000000001000000  //bit 6
#define CLEAR_ALL			0xFFFF


typedef struct {
	module *m;

	uint64_t frequency;
	uint8_t power;
	uint8_t LoRa_SF;
	uint8_t LoRa_BW;
	uint8_t LoRa_CR;
	uint8_t LoRa_CRC_sum;
	uint8_t packetLength;
	uint8_t packetType;

	//CHECK THESE DATA TYPES
	int SLEEP;
	uint8_t standby;
	int tx;
	int rx;

	uint8_t rxBuffer[MAX_PACKET_LENGTH];
	uint8_t readBytes;
} sx1280;


void config(sx1280 *s);

uint8_t GetStatus(sx1280 *s);  //status byte definitions on p73

uint8_t GetPacketType(sx1280 *s);

uint16_t GetPacketStatus(sx1280 *s);

uint8_t GetRssiInst(sx1280 *s);

void SetTxContinuousWave(sx1280 *s);

void WriteRegister(sx1280 *s, uint16_t addr, uint8_t *data, uint8_t len);

void ReadRegister(sx1280 *s, uint16_t addr, uint8_t *data, uint8_t len);

void WriteBuffer(sx1280 *s, uint8_t offset, uint8_t *data, uint8_t len);

void ReadBuffer(sx1280 *s, uint8_t offset, uint8_t *data, uint8_t len);

void SetTx(sx1280 *s);

void SetRx(sx1280 *s);

void SetRx_single(sx1280 *s);

void SetSleep(sx1280 *s, uint8_t mode);

void SetPacketType(sx1280 *s, uint8_t type);

void SetRfFrequency(sx1280 *s, uint32_t frequency);

void SetTxParams(sx1280 *s);

void SetBufferBaseAddress(sx1280 *s, uint8_t txbase, uint8_t rxbase);

void SetPacketParams(sx1280 *s); //definitions on p91

void SetModulationParams(sx1280 *s);

void SetDioIrqParams(sx1280 *s, uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask);

void SetStandby(sx1280 *s, uint8_t val);

uint16_t GetIrqStatus(sx1280 *s);

void ClearIrqStatus(sx1280 *s, uint16_t mask);

void GetRxBufferStatus(sx1280 *s, uint8_t *payloadLength, uint8_t *rxStartBufferPointer);

void set_payload(sx1280 *s, uint8_t *buffer, uint8_t size);

void get_payload(sx1280 *s, uint8_t *buffer, uint8_t *size, uint8_t maxSize);

void send_payload(sx1280 *s, uint8_t *payload, uint8_t size);

#endif /* SRC_SX1280_H_ */
