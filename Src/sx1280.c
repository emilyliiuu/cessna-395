/*
 * sx1280.c
 *
 *  Created on: Sep 30, 2025
 *      Author: 3mai1
 */

#include "sx1280.h"
#include <string.h>

void config(sx1280 *s){
    SetStandby(s, STDBY_RC);
    SetPacketType(s, PACKET_TYPE_LORA);
    SetRfFrequency(s, FREQ_2_4);
    SetBufferBaseAddress(s, TX_BASEADDR, RX_BASEADDR);
    SetModulationParams(s);
    uint8_t buf[1] = {0x37}; //sf7
    write_registers(s->m, 0x925, buf, 1);
    buf[0] = 0x01;
    write_registers(s->m, 0x093C, buf, 1);
    SetPacketParams(s);
    SetTxParams(s);

    //set sync word
    uint8_t sync[1] = {0xA4};
    write_registers(s->m, 0x944, sync, 1);
	write_registers(s->m, 0x945, sync, 1);
}

uint8_t GetStatus(sx1280 *s){
	uint8_t status = 0x00;
	read_command(s->m, GET_STATUS, (uint8_t*)&status, 1);
    return status;
}

uint8_t GetPacketType(sx1280 *s){
	uint8_t type[3] = {};
	read_command(s->m, GET_PACKET_TYPE, type, 3);
	return type[2];
}

uint8_t GetRssiInst(sx1280 *s){
	uint8_t rssi[1] = {};
	read_command(s->m, GET_RSSI_INST, rssi, 1);
	return rssi[0];
}

void SetStandby(sx1280 *s, uint8_t mode){
	write_command(s->m, SET_STANDBY, (uint8_t *)&mode, 1);
	s->standby = mode;
}

void SetSleep(sx1280 *s, uint8_t mode){
	write_command(s->m, SET_SLEEP, (uint8_t *)&mode, 1);
}

void SetPacketType(sx1280 *s, uint8_t type){
	write_command(s->m, SET_PACKET_TYPE, (uint8_t *)&type, 1);
	s->packetType = type;
}

void SetTxParams(sx1280 *s){
	uint8_t buf[2];

	buf[0] = TX_OUTPUT_POWER;
	buf[1] = RADIO_RAMP_02_US;
	write_command(s->m, SET_TX_PARAMS, buf, 2);
}

void SetRfFrequency(sx1280 *s, uint32_t frequency )
{
    uint8_t buf[3];
    uint32_t freq = 0;

    freq = (uint32_t)((double)frequency/(double)FREQ_STEP);
    buf[0] = (uint8_t)((freq>>16) & 0xFF);
    buf[1] = (uint8_t)((freq>>8) & 0xFF);
    buf[2] = (uint8_t)(freq & 0xFF);
    write_command(s->m, SET_RF_FREQUENCY, buf, 3);
}

void SetBufferBaseAddress(sx1280 *s, uint8_t txbase, uint8_t rxbase){
	uint8_t buf[2];
	buf[0] = txbase;
	buf[1] = rxbase;
	write_command(s->m, SET_BUFFER_BASE_ADDR, buf, 2);
}

void SetTx(sx1280 *s){
	uint8_t buf[3];
	buf[0] = 0x00;
	buf[1] = 0xFF;
	buf[2] = 0xFF;

	ClearIrqStatus(s, CLEAR_ALL);

	write_command(s->m, SET_TX, buf, 3);
	s->tx = 1;
}

void SetRx(sx1280 *s){
	uint8_t buf[3];
	buf[0] = 0x00;
	buf[1] = 0xFF;
	buf[2] = 0xFF;

	uint16_t mask = 0xFFFF;
	ClearIrqStatus(s, mask);

	write_command(s->m, SET_RX, buf, 3);
	s->tx = 0;
}

void SetRx_single(sx1280 *s){
	uint8_t buf[3];
	buf[0] = 0x00;
	buf[1] = 0x00;
	buf[2] = 0x00;

	uint16_t mask = 0xFFFF;
	ClearIrqStatus(s, mask);

	write_command(s->m, SET_RX, buf, 3);
	s->tx = 0;
}

void SetModulationParams(sx1280 *s){
	uint8_t buf[3];
	buf[0] = SF7;
	buf[1] = BW_1600;
	buf[2] = CR_LI_4_5;
	write_command(s->m, SET_MODULATION_PARAMS, buf, 3);
}

void SetPacketParams(sx1280 *s){
	uint8_t buf[7];
	buf[0] = PREAMBLE_12;
	buf[1] = HEADER_EXPLICIT;
	buf[2] = PAYLOAD_LENGTH;
	buf[3] = CRC_ON;
	buf[4] = IQ_NORMAL;
	buf[5] = 0x00;
	buf[6] = 0x00;
	write_command(s->m, SET_PACKET_PARAMS, buf, 7 );
}


void SetDioIrqParams(sx1280 *s, uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask){
	uint8_t buf[8];

	buf[0] = (uint8_t)((irqMask >> 8) & 0x00FF);
	buf[1] = (uint8_t)(irqMask & 0x00FF);
	buf[2] = (uint8_t)((dio1Mask >> 8) & 0x00FF);
	buf[3] = (uint8_t)(dio1Mask & 0x00FF);
	buf[4] = (uint8_t)((dio2Mask >> 8) & 0x00FF);
	buf[5] = (uint8_t)(dio2Mask & 0x00FF);
	buf[6] = (uint8_t)((dio3Mask >> 8) & 0x00FF);
	buf[7] = (uint8_t)(dio3Mask & 0x00FF);
	write_command(s->m, SET_DIO_IRQ_PARAMS, buf, 8 );
}

uint16_t GetIrqStatus(sx1280 *s){
	uint8_t irq[2];
	read_command(s->m, GET_IRQ_STATUS, irq, 2 );
	return (((uint16_t) irq[0]) << 8 ) | (uint16_t)irq[1];
}

void ClearIrqStatus(sx1280 *s, uint16_t mask){
	uint8_t buf[2];
	buf[0] = (uint8_t)((mask >> 8) & 0x00FF);
	buf[1] = (uint8_t)(mask & 0x00FF);
	write_command(s->m, CLEAR_IRQ_STATUS, buf, 2 );
}

uint16_t GetPacketStatus(sx1280 *s){
	uint8_t packetstatus[2];
	read_command(s->m, GET_PACKET_STATUS, packetstatus, 2 );
	return (packetstatus[0] << 8 ) | packetstatus[1];
}

void SetTxContinuousWave(sx1280 *s){
	uint8_t buf[1];
	write_command(s->m, TX_CONTINUOUS_WAVE, buf, 0);
}

void GetRxBufferStatus(sx1280 *s, uint8_t *payloadLength, uint8_t *rxStartBufferPointer ){
    uint8_t status[2];

    read_command(s->m, GETRXBUFFERSTATUS, status, 2 );
	payloadLength = status[0];
    rxStartBufferPointer[0] = status[1];
}

void set_payload(sx1280 *s, uint8_t *buffer, uint8_t size){
    write_buffer(s->m, TX_BASEADDR, buffer, size);
}

void get_payload(sx1280 *s, uint8_t *buffer, uint8_t *size , uint8_t maxSize){
    uint8_t offset[1];

    GetRxBufferStatus(s, size, offset);
    if(*size > maxSize){
    	return;
    }
    read_buffer(s->m, offset[0], buffer, *size );
}

void send_payload(sx1280 *s, uint8_t *payload, uint8_t size){
    set_payload(s, payload, size);
    SetTx(s);
}





