/*
 * File:   can.c
 * Author: alrekr
 *
 * Created on November 22, 2014, 2:11 PM
 */
#define FUCKED
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "can.h"
#include "common.h"
#include "master.h"
#include "state_machine.h"
#include <fstream>
/*
 *
 */

int8_t can_buffer_id;
Can_message emerg_stop;
Can_message msg_request_master_status;
Can_message msg_balance;
Can_message msg;
Can_message req_volt;
Can_message volt_data;
int8_t i_can;
Can_message req_temp;
Can_message temp_data;
Can_message stpm_msg;
bool return_value;
Can_message msg_assign_slave_id;
int8_t ecan1msgBuf[N_TX_BUFFER][8];
Can_message dispatched_message;
extern std::ofstream output;

void init_can_io(void) {
	/******************************************************************************
	 * Set up IO ports for ECAN module 1. Pin 3/RP87 as Tx, pin 5/RP119 as Rx.
	 * Via Section 10, Microchip dsPI33E/PI24E Family Reference Manual
	 * Input: None
	 * Output: None
	 ******************************************************************************/
	/*PMD1bits.C1MD = 0; //ECAN1 module enabled
	//C1CTRL1bits.REQOP = 4;
	//while (C1CTRL1bits.OPMODE != 4);
	C1CTRL1bits.ABAT = 0;
	C1CTRL1bits.CSIDL = 0;
	C1CTRL1bits.CANCKS = 0;*/
	//C1CTRL1bits.REQOP = 0;
	//while (C1CTRL1bits.OPMODE != 0);

#ifdef FUCKED
	/* For completely fucked board */
	/*__builtin_write_OSCCONL(OSCCON & ~(1 << 6));
	//Input Name    Function Name    Register    Configuration Bits
	//CAN1 Receive  C1RX             RPINR26     C1RXR<6:0>
	LATBbits.LATB8 = 0;
	RPINR26bits.C1RXR = 0x26; //RPI38, pin 17 (0d38 = 0x26)*/
#else
	/* For less fucked board */
	/*__builtin_write_OSCCONL(OSCCON & ~(1 << 6));
	//Input Name    Function Name    Register    Configuration Bits
	//CAN1 Receive  C1RX             RPINR26     C1RXR<6:0>
	LATGbits.LATG7 = 0;
	RPINR26bits.C1RXR = 0x77; //RPI119, pin 5 (0d119 = 0x77)*/
#endif

	/* For both boards */
	//Function    RPnR<5:0>    Output Name
	//C1TX        001110       RPn tied to CAN1 Transmit
	/*LATEbits.LATE7 = 0;
	RPOR6bits.RP87R = 0xE; //RP87, pin 3 (0d14 = 0xE)
	__builtin_write_OSCCONL(OSCCON | (1 << 6));*/
}

void init_can(void) {
	/**************************************************************************
	 * Initialisation of ECAN module 1
	 * Via Section 21, Microchip dsPI33E/PI24E Family Reference Manual
	 * Input: None
	 * Output: None
	 *************************************************************************/
	can_buffer_id = 0;
	/* Set up ECAN1 module */
	/*C1CTRL1bits.REQOP = 4;
	while (C1CTRL1bits.OPMODE != 4);
	init_can_io();*/
	/* Set up clock */
	/*C1CTRL1bits.CANCKS = 0x0;*/ /* Fcan = Fp */
	/*C1CFG2bits.SEG1PH = 0x7;
	C1CFG2bits.SEG2PHTS = 0x1;
	C1CFG2bits.SEG2PH = 0x5;
	C1CFG2bits.PRSEG = 0x4;
	C1CFG2bits.SAM = 0x1;
	C1CFG1bits.SJW = 0x3;
	C1CFG1bits.BRP = 0x0;*/

	/* Set up FIFO */
	/*C1FCTRLbits.FSA = 0x8;
	C1FCTRLbits.DMABS = 0b110;*/

	/* Go to normal mode */
	/*C1CTRL1bits.REQOP = 0;
	while (C1CTRL1bits.OPMODE != 0);

	init_dma0();
	init_dma2();*/
	/* CAN transmit/receive message control */
	/*C1RXFUL1 = C1RXFUL2 = C1RXOVF1 = C1RXOVF2 = 0x0000;
	C1TR01CONbits.TXEN0 = 1;
	C1TR01CONbits.TXEN1 = 1;
	C1TR23CONbits.TXEN2 = 1;
	C1TR23CONbits.TXEN3 = 1;
	C1TR45CONbits.TXEN4 = 1;
	C1TR45CONbits.TXEN5 = 1;
	C1TR67CONbits.TXEN6 = 1;
	C1TR67CONbits.TXEN7 = 1;


	C1TR01CONbits.TX0PRI = 0b11;
	C1TR01CONbits.TX1PRI = 0b11;
	C1TR23CONbits.TX2PRI = 0b11;
	C1TR23CONbits.TX3PRI = 0b11;
	C1TR45CONbits.TX4PRI = 0b11;
	C1TR45CONbits.TX5PRI = 0b11;
	C1TR67CONbits.TX6PRI = 0b11;
	C1TR67CONbits.TX7PRI = 0b11;*/

	/* Important bit in message_received */
	message_received.handling_in_process = false;
	interrupt_state = FSM_INVALID_STATE;
	/*can_enable_interrupt();*/
}

void init_dma0(void) {
	/**************************************************************************
	 * Set up DMA0 for Can1 TX
	 * Input: None
	 * Output: None
	 **************************************************************************/
	/*DMA0CONbits.AMODE = 0b10;
	DMA0CONbits.MODE = 0b00;
	DMA0CONbits.DIR = 0b1;
	DMA0PAD = 0x0442;
	DMA0CNT = 0x0007;
	DMA0REQbits.IRQSEL = 0b01000110;

	DMA0STAL = __builtin_dmaoffset(ecan1msgBuf);
	DMA0STAH = __builtin_dmapage(ecan1msgBuf);

	DMA0CONbits.CHEN = 1;*/
}

void init_dma2(void) {
	/**************************************************************************
	 * Set up DMA2 for Can1 RX
	 * Input: None
	 * Output: None
	 **************************************************************************/
	/*DMA2CONbits.AMODE = 0b10;
	DMA2CONbits.MODE = 0b00;
	DMA2CONbits.DIR = 0b0;

	DMA2STAL = __builtin_dmaoffset(ecan1msgBuf);
	DMA2STAH = __builtin_dmapage(ecan1msgBuf);

	DMA2CONbits.CHEN = 1;*/
}

void can_enable_interrupt(void) {
	/**************************************************************************
	 * Enables interrupt for ECAN1 module.
	 * Input: None
	 * Output: None
	 **************************************************************************/
	/*IEC2bits.C1IE = 1;
	C1INTEbits.TBIE = 1;
	C1INTEbits.RBIE = 1;*/
}

void can_disable_interrupt(void) {
	/**************************************************************************
	 * Disables interrupt for ECAN1 module.
	 * Input: None
	 * Output: None
	 **************************************************************************/
	/*IEC2bits.C1IE = 0;
	C1INTEbits.TBIE = 0;
	C1INTEbits.RBIE = 0;*/
}

void send_emergency_stop(int8_t subject) {
	/**************************************************************************
	 * Send emergency stop to master; cell is out of SOA!
	 * Input: None
	 * Output: None
	 **************************************************************************/
	emerg_stop.sid = EMERGENCY_STOP_ID;
	emerg_stop.ide = 0;
	emerg_stop.rtr = 1;
	emerg_stop.rb0 = 0;
	emerg_stop.dlc = 3;
	switch (subject) {
	case EMERGENCY_OVER_VOLTAGE:
		emerg_stop.byte[0] = s_par.cell_id_max_voltage;
		emerg_stop.byte[1] = EMERGENCY_VOLTAGE;
		emerg_stop.byte[2] = s_par.Vmax;
		break;
	case EMERGENCY_UNDER_VOLTAGE:
		emerg_stop.byte[0] = s_par.cell_id_min_voltage;
		emerg_stop.byte[1] = EMERGENCY_VOLTAGE;
		emerg_stop.byte[2] = s_par.Vmin;
		break;
	case EMERGENCY_OVER_TEMPERATURE:
		emerg_stop.byte[0] = s_par.cell_id_max_temp;
		emerg_stop.byte[1] = EMERGENCY_TEMPERATURE;
		emerg_stop.byte[2] = s_par.Tmax;
		break;
	case EMERGENCY_UNDER_TEMPERATURE:
		emerg_stop.byte[0] = s_par.cell_id_max_temp;
		emerg_stop.byte[1] = EMERGENCY_TEMPERATURE;
		emerg_stop.byte[2] = s_par.Tmin;
		break;
	}
	emerg_stop.byte[0] |= s_par.slave_id << 3;
	UPDATE_CAN_BUFFER_ID;
	emerg_stop.buffer = can_buffer_id;

	write_message(&emerg_stop);
}

void request_master_status(int16_t *_timestamp) {
	/**************************************************************************
	 * Asks on the CAN bus if a master is present.
	 * Input: uint16_t timestamp, updates the time variable used by master
	 * module for watchdog purposes
	 * Output: None
	 **************************************************************************/
	msg_request_master_status.rtr = 1;
	msg_request_master_status.ide = 0;
	msg_request_master_status.rb0 = 0;
	msg_request_master_status.sid = MASTER_REQUEST_ID;
	msg_request_master_status.dlc = 0;
	UPDATE_CAN_BUFFER_ID;
	msg_request_master_status.buffer = can_buffer_id;
	write_message(&msg_request_master_status);
	system_time_tick = *_timestamp;
	s_par.expecting_reply = true;
}

void slave_request_balancing(bool do_balancing) {
	/**************************************************************************
	 * Request balancing from master
	 * Input: Bool indicating if balancing should activate or not
	 * Output: None
	 **************************************************************************/
	if (do_balancing == true) {
		msg_balance.sid = BALANCE_ID + s_par.slave_id;
	} else {
		msg_balance.sid = BALANCE_DONE_ID + s_par.slave_id;
	}
	msg_balance.ide = 0;
	msg_balance.rtr = 1;
	msg_balance.rb0 = 0;
	msg_balance.dlc = 0;
	UPDATE_CAN_BUFFER_ID;
	msg_balance.buffer = can_buffer_id;
	write_message(&msg_balance);
}

void write_charge_status(int8_t charge_status) {
	/**************************************************************************
	 * Master function: Updates the state of charging to slave, including
	 * maximum voltage per cell.
	 * Input:   Charge status
	 *              00: Charge
	 *              01: Discharge
	 *              10: Balancing
	 *              11: Reserved
	 * Output: None
	 **************************************************************************/
	msg.dlc = 4;
	msg.rtr = 0;
	msg.sid = 0x600;
	msg.rb0 = 0;
	msg.ide = 0;
	msg.byte[3] = s_par.abs_min_voltage;
	msg.byte[2] = s_par.abs_min_voltage >> 8;
	msg.byte[1] = s_par.abs_max_voltage;
	msg.byte[0] = s_par.abs_max_voltage >>8;
	msg.byte[0] |= charge_status << 6;
	UPDATE_CAN_BUFFER_ID;
	msg.buffer = can_buffer_id;
	write_message(&msg);
}

void request_voltage_data(void) {
	/**************************************************************************
	 * Request slaves to send voltage data.
	 * Input: None
	 * Output: None
	 **************************************************************************/
	req_volt.rtr = 1;
	req_volt.ide = 0;
	req_volt.rb0 = 0;
	req_volt.sid = VOLTAGE_ID;
	UPDATE_CAN_BUFFER_ID;
	req_volt.buffer = can_buffer_id;
	write_message(&req_volt);
}

void write_voltage_data(void) {
	/**************************************************************************
	 * Send voltage data. Implements write_message function
	 * Input: None
	 * Output: None
	 **************************************************************************/
	for (i_can = 0; i_can < 8; i_can++) {
		volt_data.byte[i_can] = _round(s_par.voltage[i_can] * CONVERT_12_TO_8_BIT_RATIO);
	}
	volt_data.sid = VOLTAGE_ID + s_par.slave_id;
	volt_data.ide = 0;
	volt_data.rtr = 0;
	volt_data.rb0 = 0;
	volt_data.dlc = 8;
	UPDATE_CAN_BUFFER_ID;
	volt_data.buffer = can_buffer_id;
	write_message(&volt_data);
}

void request_temperature_data(void) {
	/**************************************************************************
	 * Request slaves to send temperature data.
	 * Input: None
	 * Output: None
	 **************************************************************************/
	req_temp.rtr = 1;
	req_temp.ide = 0;
	req_temp.rb0 = 0;
	req_temp.sid = TEMPERATURE_ID;
	UPDATE_CAN_BUFFER_ID;
	req_temp.buffer = can_buffer_id;
	write_message(&req_temp);
}

void write_temperature_data(void) {
	/**************************************************************************
	 * Send temperature data.
	 * Input: None
	 * Output: None
	 **************************************************************************/
	for (i_can = 0; i_can < 8; i_can++) {
		temp_data.byte[i_can] = _round(s_par.temperature[i_can] * CONVERT_12_TO_8_BIT_RATIO);
	}
	temp_data.sid = TEMPERATURE_ID + s_par.slave_id;
	temp_data.ide = 0;
	temp_data.rtr = 0;
	temp_data.rb0 = 0;
	temp_data.dlc = 8;
	UPDATE_CAN_BUFFER_ID;
	temp_data.buffer = can_buffer_id;
	write_message(&temp_data);
}

void write_stpm_data(void) {
	/**************************************************************************
	 * Writes all STPM data to bus. Each message takes no longer than 133 µs to
	 * send. 8 messages in queue thus takes no longer than 1.064 ms to send.
	 * This routine attempts to handle any bottleneck, but only after 8
	 * messages, assuming no messages are waiting to be sent. Considering the
	 * CAN IO action is in a slower time domain than the MCU, the whole process
	 * should not last more than 1.5 ms.
	 * Input: None
	 * Output: None
	 **************************************************************************/
	/* Values which aren't changed during this routine */
	stpm_msg.rtr = 0;
	stpm_msg.rb0 = 0;
	stpm_msg.ide = 0;

	/* First message in STPM */
	stpm_msg.sid = CAN_STPM0;
	stpm_msg.dlc = 8;

	for (i_can = 0; i_can < 8; i_can++) {
		stpm_msg.byte[i_can] = stpm_data.comp[i_can];
	}

	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);

	/* Second message in STPM */
	stpm_msg.sid = CAN_STPM1;
	stpm_msg.dlc = 8;

	for (i_can = 0; i_can < 8; i_can++) {
		stpm_msg.byte[i_can] = stpm_data.rev[i_can];
	}

	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);

	/* Third message in STPM */
	stpm_msg.sid = CAN_STPM2;
	stpm_msg.dlc = 6;
	stpm_msg.byte[0] = stpm_data.state;
	stpm_msg.byte[1] = stpm_data.timer >> 8;
	stpm_msg.byte[2] = stpm_data.timer;
	stpm_msg.byte[3] = stpm_data.flags;
	stpm_msg.byte[4] = stpm_data.dtc1;
	stpm_msg.byte[5] = stpm_data.dtc2;
	stpm_msg.byte[6] = STPM_INVALID_8_U;
	stpm_msg.byte[7] = STPM_INVALID_8_U;
	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);

	/* Fourth message in STPM */
	stpm_msg.sid = CAN_STPM3;
	stpm_msg.dlc = 6;
	stpm_msg.byte[0] = stpm_data.voltage >> 8;
	stpm_msg.byte[1] = stpm_data.voltage;
	stpm_msg.byte[2] = stpm_data.min_volt;
	stpm_msg.byte[3] = stpm_data.min_volt_n;
	stpm_msg.byte[4] = stpm_data.max_volt;
	stpm_msg.byte[5] = stpm_data.max_volt_n;
	stpm_msg.byte[6] = STPM_INVALID_8_U;
	stpm_msg.byte[7] = STPM_INVALID_8_U;
	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);

	/* Fifth message in STPM */
	stpm_msg.sid = CAN_STPM4;
	stpm_msg.dlc = 6;
	stpm_msg.byte[0] = stpm_data.current >> 8;
	stpm_msg.byte[1] = stpm_data.current;
	stpm_msg.byte[2] = stpm_data.charge_limit >> 8;
	stpm_msg.byte[3] = stpm_data.charge_limit;
	stpm_msg.byte[4] = stpm_data.discharge_limit >> 8;
	stpm_msg.byte[5] = stpm_data.discharge_limit;
	stpm_msg.byte[6] = STPM_INVALID_8_U;
	stpm_msg.byte[7] = STPM_INVALID_8_U;
	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);

	/* Sixth message in STPM */
	stpm_msg.sid = CAN_STPM5;
	stpm_msg.dlc = 8;
	stpm_msg.byte[0] = stpm_data.batt_energy_in0 >> 8;
	stpm_msg.byte[1] = stpm_data.batt_energy_in0;
	stpm_msg.byte[2] = stpm_data.batt_energy_in1 >> 8;
	stpm_msg.byte[3] = stpm_data.batt_energy_in1;
	stpm_msg.byte[4] = stpm_data.batt_energy_out0 >> 8;
	stpm_msg.byte[5] = stpm_data.batt_energy_out0;
	stpm_msg.byte[6] = stpm_data.batt_energy_out1 >> 8;
	stpm_msg.byte[7] = stpm_data.batt_energy_out1;
	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);

	/* Seventh message in STPM */
	stpm_msg.sid = CAN_STPM6;
	stpm_msg.dlc = 7;
	stpm_msg.byte[0] = stpm_data.soc;
	stpm_msg.byte[1] = stpm_data.dod >> 8;
	stpm_msg.byte[2] = stpm_data.dod;
	stpm_msg.byte[3] = stpm_data.capacity >> 8;
	stpm_msg.byte[4] = stpm_data.capacity;
	stpm_msg.byte[5] = stpm_data.soc2;
	stpm_msg.byte[6] = stpm_data.soh;
	stpm_msg.byte[7] = STPM_INVALID_8_U;
	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);

	/* Eigth message in STPM */
	stpm_msg.sid = CAN_STPM7;
	stpm_msg.dlc = 6;
	stpm_msg.byte[0] = stpm_data.temperature;
	stpm_msg.byte[1] = stpm_data.air_temp;
	stpm_msg.byte[2] = stpm_data.min_temp;
	stpm_msg.byte[3] = stpm_data.min_temp_n;
	stpm_msg.byte[4] = stpm_data.max_temp;
	stpm_msg.byte[5] = stpm_data.max_temp_n;
	stpm_msg.byte[6] = STPM_INVALID_8_U;
	stpm_msg.byte[7] = STPM_INVALID_8_U;
	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);

	//while (messages_in_queue > 6); //wait until three buffers are available

	/* Ninth message in STPM */
	stpm_msg.sid = CAN_STPM8;
	stpm_msg.dlc = 6;
	stpm_msg.byte[0] = stpm_data.resistance >> 8;
	stpm_msg.byte[1] = stpm_data.resistance;
	stpm_msg.byte[2] = stpm_data.min_res;
	stpm_msg.byte[3] = stpm_data.min_res_no;
	stpm_msg.byte[4] = stpm_data.max_res;
	stpm_msg.byte[5] = stpm_data.max_res_no;
	stpm_msg.byte[6] = STPM_INVALID_8_U;
	stpm_msg.byte[7] = STPM_INVALID_8_U;
	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);

	/* Tenth message in STPM */
	stpm_msg.sid = CAN_STPM9;
	stpm_msg.dlc = 8;
	stpm_msg.byte[0] = stpm_data.ac_line_voltage >> 8;
	stpm_msg.byte[1] = stpm_data.ac_line_voltage;
	stpm_msg.byte[2] = stpm_data.ac_line_current >> 8;
	stpm_msg.byte[3] = stpm_data.ac_line_current;
	stpm_msg.byte[4] = stpm_data.max_charge_current;
	stpm_msg.byte[5] = stpm_data.max_discharge_current;
	stpm_msg.byte[6] = stpm_data.ac_line_power >> 8;
	stpm_msg.byte[7] = stpm_data.ac_line_power;
	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);

	/* Eleventh message in STPM */
	stpm_msg.sid = CAN_STPM10;
	stpm_msg.dlc = 8;
	stpm_msg.byte[0] = stpm_data.ac_energy_in0 >> 8;
	stpm_msg.byte[1] = stpm_data.ac_energy_in0;
	stpm_msg.byte[2] = stpm_data.ac_energy_in1 >> 8;
	stpm_msg.byte[3] = stpm_data.ac_energy_in1;
	stpm_msg.byte[4] = stpm_data.ac_energy_out0 >> 8;
	stpm_msg.byte[5] = stpm_data.ac_energy_out0;
	stpm_msg.byte[6] = stpm_data.ac_energy_out1 >> 8;
	stpm_msg.byte[7] = stpm_data.ac_energy_out1;
	UPDATE_CAN_BUFFER_ID;
	stpm_msg.buffer = can_buffer_id;
	write_message(&stpm_msg);
	reset_stpm_data();
}

void write_message(Can_message *_msg) {
	/**************************************************************************
	 * Writes message data to relevant registers and asks for message to be
	 * sent. Can be used for standard data frame and standard remote frame.
	 * Extended data frames needs to be implemented, if they are to be used.
	 * Input: Pointer to struct Can_message containing all relevant data
	 * Output: None
	 **************************************************************************/

	_msg->_arbitration = 0x00;
	_msg->_arbitration |= _msg->rtr;
	_msg->_arbitration |= _msg->sid << 1;
	ecan1msgBuf[_msg->buffer][0] = _msg->_arbitration;

	ecan1msgBuf[_msg->buffer][1] = 0x0000; //EID bits - this CAN message is not extended
	_msg->_control = 0x00;
	_msg->_control = _msg->dlc;
	_msg->_control |= _msg->rb0 << 4;
	_msg->_control |= _msg->ide << 5;
	ecan1msgBuf[_msg->buffer][2] = _msg->_control;

	switch (_msg->dlc) {
	case 8:
		_msg->_data76 |= _msg->byte[7] << 8;
	case 7:
		_msg->_data76 |= _msg->byte[6];
		ecan1msgBuf[_msg->buffer][3] = _msg->_data10;
	case 6:
		_msg->_data54 |= _msg->byte[5] << 8;
	case 5:
		_msg->_data54 |= _msg->byte[4];
		ecan1msgBuf[_msg->buffer][4] = _msg->_data32;
	case 4:
		_msg->_data32 |= _msg->byte[3] << 8;
	case 3:
		_msg->_data32 |= _msg->byte[2];
		ecan1msgBuf[_msg->buffer][5] = _msg->_data54;
	case 2:
		_msg->_data10 |= _msg->byte[1] << 8;
	case 1:
		_msg->_data10 |= _msg->byte[0];
		ecan1msgBuf[_msg->buffer][6] = _msg->_data76;
		break;
	default: // Silently discard incorrect packages
		break;
	}
	switch (_msg->buffer) {
	case BUFFER0:
		messages_in_queue++;
		/*C1TR01CONbits.TXREQ0 = 0x1;*/
		break;
	case BUFFER1:
		messages_in_queue++;
		/*C1TR01CONbits.TXREQ1 = 0x1;*/
		break;
	case BUFFER2:
		messages_in_queue++;
		/*C1TR23CONbits.TXREQ2 = 0x1;*/
		break;
	case BUFFER3:
		messages_in_queue++;
		/*C1TR23CONbits.TXREQ3 = 0x1;*/
		break;
	case BUFFER4:
		messages_in_queue++;
		/*C1TR45CONbits.TXREQ4 = 0x1;*/
		break;
	case BUFFER5:
		messages_in_queue++;
		/*C1TR45CONbits.TXREQ5 = 0x1;*/
		break;
	default: // Silently discard incorrect packages
		break;
	}

	fprintf(f_output, "Input                                                    Output\n");

	fprintf(f_output, "SID|RTR|IDE|RB0|DLC|DATA                                 SID|RTR|IDE|RB0|DLC|DATA\n");
	fprintf(f_output, "%3x|%3d|%3d|%3d|%3d", dispatched_message.sid, dispatched_message.rtr, dispatched_message.ide, dispatched_message.rb0, dispatched_message.dlc);
	if (_msg->dlc > 0) {
		fprintf(f_output, "|   ");
	} else {
		fprintf(f_output, "    ");
	}
	for (i_can = 0; i_can < _msg->dlc; i_can++) {
		fprintf(f_output, " %3d", (int)dispatched_message.byte[i_can]);
	}
	for(; i_can < 8; i_can++) {
		fprintf(f_output, "    ");
	}

	fprintf(f_output, "  %3x|%3d|%3d|%3d|%3d", _msg->sid, _msg->rtr,_msg->ide, _msg->rb0, _msg->dlc);
	if (_msg->dlc > 0) {
		fprintf(f_output, "|   ");
	} else {
		fprintf(f_output, "    ");
	}
	for (i_can = 0; i_can < _msg->dlc; i_can++) {
		fprintf(f_output, " %3d", (int)_msg->byte[i_can]);
	}
	for(; i_can < 8; i_can++) {
		fprintf(f_output, "    ");
	}
	fprintf(f_output, "\n");
}

bool handle_message() {
	/**************************************************************************
	 * Dispatches the received message.
	 * Input: None
	 * Output: None
	 **************************************************************************/
	dispatched_message = message_received;

	if (dispatched_message.sid - 0x020 > 0 && dispatched_message.sid - 0x20 < 32) {
		s_par.reply_from_master_request_seen = true;
	}

	return true;
}

void send_slave_id(int8_t id) {
	/**************************************************************************
	 * Send a slave ID to a slave on the bus.
	 * Input: ID
	 * Output: None
	 **************************************************************************/
	msg_assign_slave_id.sid = MASTER_REPLY_ID + id;
	msg_assign_slave_id.dlc = 0;
	msg_assign_slave_id.ide = 0;
	msg_assign_slave_id.rtr = 1;
	msg_assign_slave_id.rb0 = 0;
	write_message(&msg_assign_slave_id);
}

int _round(double val) {
	/**************************************************************************
	 * Rounds a positive double to the nearest int
	 * Input: double value to be rounded
	 * Output: integer value, rounded
	 **************************************************************************/
	return val > 0 ? (int)(val + 0.5) : (int)(val - 0.5);
}
