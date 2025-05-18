/*
 * LoRa_i.c
 *
 *  Created on: Nov 1, 2024
 *      Author: subhr
 */
#include "LoRa_i.h"
LoRa_i newLoRa_i(){
	LoRa_i new_LoRa;

	new_LoRa.frequency             = 915       ;
	new_LoRa.spredingFactor        = SF_12      ;
	new_LoRa.bandWidth			   = BW_125KHz ;
	new_LoRa.crcRate               = CR_4_5    ;
	new_LoRa.power				   = POWER_20db;
	new_LoRa.overCurrentProtection = 100       ;
	new_LoRa.preamble			   = 8         ;

	return new_LoRa;
}
/* ----------------------------------------------------------------------------- *\
		name        : LoRa_reset_i

		description : reset module

		arguments   :
			LoRa* LoRa --> LoRa object handler

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_reset_i(LoRa_i* _LoRa){
	HAL_GPIO_WritePin(_LoRa->reset_port, _LoRa->reset_pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(_LoRa->reset_port, _LoRa->reset_pin, GPIO_PIN_SET);
	HAL_Delay(100);
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_gotoMode_i

		description : set LoRa Op mode

		arguments   :
			LoRa* LoRa    --> LoRa object handler
			mode	        --> select from defined modes

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_gotoMode_i(LoRa_i* _LoRa, int mode){
	uint8_t    read;
	uint8_t    data;

	read = LoRa_read_i(_LoRa, RegOpMode);
	data = read;

	if(mode == SLEEP_MODE){
		data = (read & 0xF8) | 0x00;
		_LoRa->current_mode = SLEEP_MODE;
	}else if (mode == STNBY_MODE){
		data = (read & 0xF8) | 0x01;
		_LoRa->current_mode = STNBY_MODE;
	}else if (mode == TRANSMIT_MODE){
		data = (read & 0xF8) | 0x03;
		_LoRa->current_mode = TRANSMIT_MODE;
	}else if (mode == RXCONTIN_MODE){
		data = (read & 0xF8) | 0x05;
		_LoRa->current_mode = RXCONTIN_MODE;
	}else if (mode == RXSINGLE_MODE){
		data = (read & 0xF8) | 0x06;
		_LoRa->current_mode = RXSINGLE_MODE;
	}

	LoRa_write_i(_LoRa, RegOpMode, data);
	//HAL_Delay(10);
}


/* ----------------------------------------------------------------------------- *\
		name        : LoRa_read_iReg_i

		description : read a register(s) by an address and a length,
									then store value(s) at outpur array.
		arguments   :
			LoRa* LoRa        --> LoRa object handler
			uint8_t* address  -->	pointer to the beginning of address array
			uint16_t r_length -->	detemines number of addresse bytes that
														you want to send
			uint8_t* output		--> pointer to the beginning of output array
			uint16_t w_length	--> detemines number of bytes that you want to read

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_read_iReg_i(LoRa_i* _LoRa, uint8_t* address, uint16_t r_length, uint8_t* output, uint16_t w_length){
	HAL_GPIO_WritePin(_LoRa->CS_port, _LoRa->CS_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(_LoRa->hSPIx, address, r_length, TRANSMIT_TIMEOUT);
	while (HAL_SPI_GetState(_LoRa->hSPIx) != HAL_SPI_STATE_READY)
		;
	HAL_SPI_Receive(_LoRa->hSPIx, output, w_length, RECEIVE_TIMEOUT);
	while (HAL_SPI_GetState(_LoRa->hSPIx) != HAL_SPI_STATE_READY)
		;
	HAL_GPIO_WritePin(_LoRa->CS_port, _LoRa->CS_pin, GPIO_PIN_SET);
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_write_iReg_i

		description : write a value(s) in a register(s) by an address

		arguments   :
			LoRa* LoRa        --> LoRa object handler
			uint8_t* address  -->	pointer to the beginning of address array
			uint16_t r_length -->	detemines number of addresse bytes that
														you want to send
			uint8_t* output		--> pointer to the beginning of values array
			uint16_t w_length	--> detemines number of bytes that you want to send

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_write_iReg_i(LoRa_i* _LoRa, uint8_t* address, uint16_t r_length, uint8_t* values, uint16_t w_length){
	HAL_GPIO_WritePin(_LoRa->CS_port, _LoRa->CS_pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(_LoRa->hSPIx, address, r_length, TRANSMIT_TIMEOUT);
	while (HAL_SPI_GetState(_LoRa->hSPIx) != HAL_SPI_STATE_READY)
		;
	HAL_SPI_Transmit(_LoRa->hSPIx, values, w_length, TRANSMIT_TIMEOUT);
	while (HAL_SPI_GetState(_LoRa->hSPIx) != HAL_SPI_STATE_READY)
		;
	HAL_GPIO_WritePin(_LoRa->CS_port, _LoRa->CS_pin, GPIO_PIN_SET);
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_setFrequency_i

		description : set carrier frequency e.g 433 MHz

		arguments   :
			LoRa* LoRa        --> LoRa object handler
			int   freq        --> desired frequency in MHz unit, e.g 434

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_setFrequency_i(LoRa_i* _LoRa, int freq){
	uint8_t  data;
	uint32_t F;
	F = (freq * 524288)>>5;

	// write Msb:
	data = F >> 16;
	LoRa_write_i(_LoRa, RegFrMsb, data);
	HAL_Delay(5);

	// write Mid:
	data = F >> 8;
	LoRa_write_i(_LoRa, RegFrMid, data);
	HAL_Delay(5);

	// write Lsb:
	data = F >> 0;
	LoRa_write_i(_LoRa, RegFrLsb, data);
	HAL_Delay(5);
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_setSpreadingFactor_i

		description : set spreading factor, from 7 to 12.

		arguments   :
			LoRa* LoRa        --> LoRa object handler
			int   SP          --> desired spreading factor e.g 7

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_setSpreadingFactor_i(LoRa_i* _LoRa, int SF){
	uint8_t	data;
	uint8_t	read;

	if(SF>12)
		SF = 12;
	if(SF<7)
		SF = 7;

	read = LoRa_read_i(_LoRa, RegModemConfig2);
	HAL_Delay(10);

	data = (SF << 4) + (read & 0x0F);
	LoRa_write_i(_LoRa, RegModemConfig2, data);
	HAL_Delay(10);
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_setPower_i

		description : set power gain.

		arguments   :
			LoRa* LoRa        --> LoRa object handler
			int   power       --> desired power like POWER_17db

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_setPower_i(LoRa_i* _LoRa, uint8_t power){
	LoRa_write_i(_LoRa, RegPaConfig, power);
	HAL_Delay(10);
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_setOCP_i

		description : set maximum allowed current.

		arguments   :
			LoRa* LoRa        --> LoRa object handler
			int   current     --> desired max currnet in mA, e.g 120

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_setOCP_i(LoRa_i* _LoRa, uint8_t current){
	uint8_t	OcpTrim = 0;

	if(current<45)
		current = 45;
	if(current>240)
		current = 240;

	if(current <= 120)
		OcpTrim = (current - 45)/5;
	else if(current <= 240)
		OcpTrim = (current + 30)/10;

	OcpTrim = OcpTrim + (1 << 5);
	LoRa_write_i(_LoRa, RegOcp, OcpTrim);
	HAL_Delay(10);
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_setTOMsb_setCRCon_i

		description : set timeout msb to 0xFF + set CRC enable.

		arguments   :
			LoRa* LoRa        --> LoRa object handler

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_setTOMsb_setCRCon_i(LoRa_i* _LoRa){
	uint8_t read, data;

	read = LoRa_read_i(_LoRa, RegModemConfig2);

	data = read | 0x07;
	LoRa_write_i(_LoRa, RegModemConfig2, data);\
	HAL_Delay(10);
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_read_i

		description : read a register by an address

		arguments   :
			LoRa*   LoRa        --> LoRa object handler
			uint8_t address     -->	address of the register e.g 0x1D

		returns     : register value
\* ----------------------------------------------------------------------------- */
uint8_t LoRa_read_i(LoRa_i* _LoRa, uint8_t address){
	uint8_t read_data;
	uint8_t data_addr;

	data_addr = address & 0x7F;
	LoRa_read_iReg_i(_LoRa, &data_addr, 1, &read_data, 1);
	//HAL_Delay(5);

	return read_data;
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_write_i

		description : write a value in a register by an address

		arguments   :
			LoRa*   LoRa        --> LoRa object handler
			uint8_t address     -->	address of the register e.g 0x1D
			uint8_t value       --> value that you want to write

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_write_i(LoRa_i* _LoRa, uint8_t address, uint8_t value){
	uint8_t data;
	uint8_t addr;

	addr = address | 0x80;
	data = value;
	LoRa_write_iReg_i(_LoRa, &addr, 1, &data, 1);
	//HAL_Delay(5);
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_BurstWrite_i

		description : write a set of values in a register by an address respectively

		arguments   :
			LoRa*   LoRa        --> LoRa object handler
			uint8_t address     -->	address of the register e.g 0x1D
			uint8_t *value      --> address of values that you want to write

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_BurstWrite_i(LoRa_i* _LoRa, uint8_t address, uint8_t *value, uint8_t length){
	uint8_t addr;
	addr = address | 0x80;

	//NSS = 1
	HAL_GPIO_WritePin(_LoRa->CS_port, _LoRa->CS_pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(_LoRa->hSPIx, &addr, 1, TRANSMIT_TIMEOUT);
	while (HAL_SPI_GetState(_LoRa->hSPIx) != HAL_SPI_STATE_READY)
		;
	//Write data in FiFo
	HAL_SPI_Transmit(_LoRa->hSPIx, value, length, TRANSMIT_TIMEOUT);
	while (HAL_SPI_GetState(_LoRa->hSPIx) != HAL_SPI_STATE_READY)
		;
	//NSS = 0
	//HAL_Delay(5);
	HAL_GPIO_WritePin(_LoRa->CS_port, _LoRa->CS_pin, GPIO_PIN_SET);
}
/* ----------------------------------------------------------------------------- *\
		name        : LoRa_isvalid_i

		description : check the LoRa instruct values

		arguments   :
			LoRa* LoRa --> LoRa object handler

		returns     : returns 1 if all of the values were given, otherwise returns 0
\* ----------------------------------------------------------------------------- */
uint8_t LoRa_isvalid_i(LoRa_i* _LoRa){

	return 1;
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_transmit_i

		description : Transmit data

		arguments   :
			LoRa*    LoRa     --> LoRa object handler
			uint8_t  data			--> A pointer to the data you wanna send
			uint8_t	 length   --> Size of your data in Bytes
			uint16_t timeOut	--> Timeout in milliseconds
		returns     : 1 in case of success, 0 in case of timeout
\* ----------------------------------------------------------------------------- */
uint8_t LoRa_transmit_i(LoRa_i* _LoRa, uint8_t* data, uint8_t length, uint16_t timeout){
	uint8_t read;

	int mode = _LoRa->current_mode;
	LoRa_gotoMode_i(_LoRa, STNBY_MODE);
	read = LoRa_read_i(_LoRa, RegFiFoTxBaseAddr);
	LoRa_write_i(_LoRa, RegFiFoAddPtr, read);
	LoRa_write_i(_LoRa, RegPayloadLength, length);
	LoRa_BurstWrite_i(_LoRa, RegFiFo, data, length);
	LoRa_gotoMode_i(_LoRa, TRANSMIT_MODE);
	while(1){
		read = LoRa_read_i(_LoRa, RegIrqFlags);
		if((read & 0x08)!=0){
			LoRa_write_i(_LoRa, RegIrqFlags, 0xFF);
			LoRa_gotoMode_i(_LoRa, mode);
			return 1;
		}
		else{
			if(--timeout==0){
				LoRa_gotoMode_i(_LoRa, mode);
				return 0;
			}
		}
		HAL_Delay(1);
	}
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_startReceiving_i

		description : Start receiving continuously

		arguments   :
			LoRa*    LoRa     --> LoRa object handler

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
void LoRa_startReceiving_i(LoRa_i* _LoRa){
	LoRa_gotoMode_i(_LoRa, RXCONTIN_MODE);
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_receive_i

		description : Read received data from module

		arguments   :
			LoRa*    LoRa     --> LoRa object handler
			uint8_t  data			--> A pointer to the array that you want to write bytes in it
			uint8_t	 length   --> Determines how many bytes you want to read

		returns     : The number of bytes received
\* ----------------------------------------------------------------------------- */
uint8_t LoRa_receive_i(LoRa_i* _LoRa, uint8_t* data, uint8_t length){
	uint8_t read;
	uint8_t number_of_bytes;
	uint8_t min = 0;

	for(int i=0; i<length; i++)
		data[i]=0;

	LoRa_gotoMode_i(_LoRa, STNBY_MODE);
	read = LoRa_read_i(_LoRa, RegIrqFlags);
	if((read & 0x40) != 0){
		LoRa_write_i(_LoRa, RegIrqFlags, 0xFF);
		number_of_bytes = LoRa_read_i(_LoRa, RegRxNbBytes);
		read = LoRa_read_i(_LoRa, RegFiFoRxCurrentAddr);
		LoRa_write_i(_LoRa, RegFiFoAddPtr, read);
		min = length >= number_of_bytes ? number_of_bytes : length;
		for(int i=0; i<min; i++)
			data[i] = LoRa_read_i(_LoRa, RegFiFo);
	}
	LoRa_gotoMode_i(_LoRa, RXCONTIN_MODE);
    return min;
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_getRSSI_i

		description : initialize and set the right setting according to LoRa sruct vars

		arguments   :
			LoRa* LoRa        --> LoRa object handler

		returns     : Returns the RSSI value of last received packet.
\* ----------------------------------------------------------------------------- */
int LoRa_getRSSI_i(LoRa_i* _LoRa){
	uint8_t read;
	read = LoRa_read_i(_LoRa, RegPktRssiValue);
	return -164 + read;
}

/* ----------------------------------------------------------------------------- *\
		name        : LoRa_init_i

		description : initialize and set the right setting according to LoRa sruct vars

		arguments   :
			LoRa* LoRa        --> LoRa object handler

		returns     : Nothing
\* ----------------------------------------------------------------------------- */
uint16_t LoRa_init_i(LoRa_i* _LoRa){
	uint8_t    data;
	uint8_t    read;

	if(LoRa_isvalid_i(_LoRa)){
		// goto sleep mode:
			LoRa_gotoMode_i(_LoRa, SLEEP_MODE);
			HAL_Delay(10);

		// turn on LoRa mode:
			read = LoRa_read_i(_LoRa, RegOpMode);
			HAL_Delay(10);
			data = read | 0x80;
			LoRa_write_i(_LoRa, RegOpMode, data);
			HAL_Delay(100);

		// set frequency:
			LoRa_setFrequency_i(_LoRa, _LoRa->frequency);

		// set output power gain:
			LoRa_setPower_i(_LoRa, _LoRa->power);

		// set over current protection:
			LoRa_setOCP_i(_LoRa, _LoRa->overCurrentProtection);

		// set LNA gain:
			LoRa_write_i(_LoRa, RegLna, 0x23);

		// set spreading factor, CRC on, and Timeout Msb:
			LoRa_setTOMsb_setCRCon_i(_LoRa);
			LoRa_setSpreadingFactor_i(_LoRa, _LoRa->spredingFactor);

		// set Timeout Lsb:
			LoRa_write_i(_LoRa, RegSymbTimeoutL, 0xFF);

		// set bandwidth, coding rate and expilicit mode:
			// 8 bit RegModemConfig --> | X | X | X | X | X | X | X | X |
			//       bits represent --> |   bandwidth   |     CR    |I/E|
			data = 0;
			data = (_LoRa->bandWidth << 4) + (_LoRa->crcRate << 1);
			LoRa_write_i(_LoRa, RegModemConfig1, data);

		// set preamble:
			LoRa_write_i(_LoRa, RegPreambleMsb, _LoRa->preamble >> 8);
			LoRa_write_i(_LoRa, RegPreambleLsb, _LoRa->preamble >> 0);

		// DIO mapping:   --> DIO: RxDone
			read = LoRa_read_i(_LoRa, RegDioMapping1);
			data = read | 0x3F;
			LoRa_write_i(_LoRa, RegDioMapping1, data);

		// goto standby mode:
			LoRa_gotoMode_i(_LoRa, STNBY_MODE);
			_LoRa->current_mode = STNBY_MODE;
			HAL_Delay(10);

			read = LoRa_read_i(_LoRa, RegVersion);
			if(read == 0x12)
				return LORA_OK;
			else
				return LORA_NOT_FOUND;
	}
	else {
		return LORA_UNAVAILABLE;
	}
}
