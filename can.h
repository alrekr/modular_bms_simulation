/*
 * File:   can.h
 * Author: alrekr
 *
 * Created on November 22, 2014, 2:11 PM
 */

#ifndef CAN_H
#define	CAN_H

#include <stdint.h>
#include <stdbool.h>
#include "structs.h"
#include "slave.h"
#include "common.h"

#ifdef	__cplusplus
extern "C" {
#endif

extern int8_t can_buffer_id;
extern int8_t ecan1msgBuf[N_TX_BUFFER][8];
extern int8_t i_can;
extern Can_message emerg_stop;
extern Can_message msg_request_master_status;
extern Can_message msg_balance;
extern Can_message msg;
extern Can_message req_volt;
extern Can_message volt_data;
extern Can_message req_temp;
extern Can_message stpm_msg;
extern bool return_value;
extern Can_message msg_assign_slave_id;

void init_can_io(void);
void init_can(void);
void init_dma0(void);
void init_dma2(void);
void can_enable_interrupt(void);
void can_disable_interrupt(void);
bool handle_message();
void request_master_status(int16_t *_timestamp);
void request_temperature_data(void);
void request_voltage_data(void);
void send_emergency_stop(int8_t subject);
void send_slave_id(int8_t id);
void slave_request_balancing(bool do_balancing);
void write_charge_status(int8_t charge_status);
void write_message(Can_message *_msg);
void write_stpm_data(void);
void write_temperature_data(void);
void write_voltage_data(void);

int _round(double val);

#ifdef	__cplusplus
}
#endif

#endif	/* CAN_H */

