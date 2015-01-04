/**********************************************************************
 * (c) 2005 Microchip Technology Inc.
 *
 * FileName:        common.c
 * Processor:       dsPIC33Fxxxx
 * Compiler:        MPLAB(R) C30 v3.00 or higher
 *
 * SOFTWARE LICENSE AGREEMENT:
 * Microchip Technology Incorporated ("Microchip") retains all ownership and
 * intellectual property rights in the code accompanying this message and in all
 * derivatives hereto.  You may use this code, and any derivatives created by
 * any person or entity by or on your behalf, exclusively with Microchip's
 * proprietary products.  Your acceptance and/or use of this code constitutes
 * agreement to the terms and conditions of this notice.
 *
 * CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
 * WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
 * PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER
 * IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY),
 * STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL,
 * PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
 * ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN
 * ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO
 * THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
 * HAVE THIS CODE DEVELOPED.
 *
 * You agree that you are solely responsible for testing the code and
 * determining its suitability.  Microchip has no obligation to modify, test,
 * certify, or support the code.
 *
 * REVISION HISTORY:
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Author          	Date      Comments on this revision
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Jatinder Gharoo 	06/05/07  Modified release of the project
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ADDITIONAL NOTES:
 * This code is tested on Explorer-16 board with ECAN PICTail Card.
 * The device used is dsPIC33FJ256GP710 controller
 *************************************************************************************************/
#include <fstream>
#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

/******************************************************************************
 * Defines
 ******************************************************************************/

//#define MASTER

#define CAN_MSG_DATA                0x01 // message type
#define CAN_MSG_RTR                 0x02 // data or RTR
#define CAN_FRAME_EXT               0x03 // Frame type
#define CAN_FRAME_STD               0x04 // extended or standard

/*
#define MUX_A0_PIN                  LATB14
#define MUX_A1_PIN                  LATB15
#define MUX_A2_PIN                  LATF3
#define MUX_A0_W                    LATBbits.MUX_A0_PIN
#define MUX_A1_W                    LATBbits.MUX_A1_PIN
#define MUX_A2_W                    LATFbits.MUX_A2_PIN

#define LED_RED_PIN                 LATF5
#define LED_GREEN_PIN               LATG2
#define LED_YELLOW_PIN              LATF4

#define LED_RED_W                   LATFbits.LED_RED_PIN
#define LED_GREEN_W                 LATGbits.LED_GREEN_PIN
#define LED_YELLOW_W                LATFbits.LED_YELLOW_PIN

*/
#define N_MODULES                   24
#define N_CELLS                     8
#define N_WATCHDOG_MODULES          30

#define NUM_OF_ECAN_BUFFERS         32
#define CONVERT_12_TO_8_BIT_RATIO   16.058823529 //273/17
#define MASTER_REQUEST_ID           0x000
#define EMERGENCY_STOP_ID           0x002
#define MASTER_REPLY_ID             0x020
#define BALANCE_ID                  0x040
#define BALANCE_DONE_ID             0x060
#define CHARGE_STATUS_ID            0x600
#define VOLTAGE_ID                  0x640
#define TEMPERATURE_ID              0x660
#define CURRENT_ID                  0x680
#define MASTER_ID                   0x0

#define VOLTAGE_DIFF_THRESHOLD      0x0003 //very small, 3.663 mV (1.221 mV * 3)
/*#define DISABLE_INTERRUPTS          __asm__ volatile ("disi #0x3FFF");
#define ENABLE_INTERRUPTS           __asm__ volatile ("disi #0");*/

//999 used so no clashes can occur between FSM and CAN-states.
#define FSM_INIT                    9990
#define FSM_CHARGING                9991
#define FSM_DISCHARGING             9992
#define FSM_BALANCING               9993
#define FSM_ASSIGN_SLAVE            9994
#define FSM_SEND_STPM               9995
#define FSM_SEND_VOLTAGE_DATA       9996
#define FSM_SEND_TEMPERATURE_DATA   9997
#define FSM_HANDLE_MESSAGE          9998

#define FSM_LOC_REQUEST_MASTER      9909
#define FSM_LOC_REQ_VOLTAGE         9910
#define FSM_LOC_REQ_TEMPERATURE     9911
#define FSM_LOC_REQ_CURRENT         9912
#define FSM_LOC_REPLY_VOLTAGE       9913
#define FSM_LOC_REPLY_TEMPERATURE   9914
#define FSM_LOC_REPLY_CURRENT       9915
#define FSM_LOC_REQ_BALANCING       9916
#define FSM_LOC_BALANCING_DONE      9917
#define FSM_LOC_REPLY_FROM_MASTER   9918
#define FSM_LOC_CHARGE_STATUS       9919
#define FSM_LOC_DISCHARGE_STATUS    9920
#define FSM_LOC_BALANCING_STATUS    9921

#define FSM_INVALID_STATE           9998
#define FSM_LOC_INVALID_STATE       9999

#define CAN_REQUEST_MASTER          0x0000
#define CAN_REPLY_FROM_MASTER       0x0001
#define CAN_REQUESTED_BALANCING     0x0040
#define CAN_MSG_CHARGE_STATUS       0x0600
#define CAN_STPM0                   0x0620
#define CAN_STPM1                   0x0621
#define CAN_STPM2                   0x0622
#define CAN_STPM3                   0x0623
#define CAN_STPM4                   0x0624
#define CAN_STPM5                   0x0625
#define CAN_STPM6                   0x0626
#define CAN_STPM7                   0x0627
#define CAN_STPM8                   0x0628
#define CAN_STPM9                   0x0629
#define CAN_STPM10                  0x062A
#define CAN_VOLTAGES                0x0640
#define CAN_TEMPERATURES            0x0660
#define CAN_CURRENT                 0x0680
#define CAN_SET_CHARGING            0b00000000
#define CAN_SET_DISCHARGING         0b00000001
#define CAN_SET_BALANCING           0b00000010

#define CAN_MSG_MASK                0x07e0 //= 0b0000011111100000
#define CAN_ID_MASK                 0x001f //= 0b0000000000011111
#define CAN_CHARGE_STATUS_MASK      0xC000 //= 0b1100000000000000
#define CAN_DESIRED_CELL_VOLTAGE    0x4095 //= 0b0000111111111111

#define ECAN1_MSG_BUF_LENGTH        32

#define BUFFER0                     0
#define BUFFER1                     1
#define BUFFER2                     2
#define BUFFER3                     3
#define BUFFER4                     4
#define BUFFER5                     5
#define BUFFER6                     6
#define BUFFER7                     7
#define N_TX_BUFFER                 8
#define PREVIOUS_STATE_LENGTH       30

#define STPM_COMPANY                "MODULBMS"
#define STPM_REVISION               "v0000001"
#define STPM_INVALID_8_S            0x80;
#define STPM_INVALID_8_U            0xFF;
#define STPM_INVALID_16_S           0x8000;
#define STPM_INVALID_16_U           0xFFFF;

#define EMERGENCY_OVER_VOLTAGE      0
#define EMERGENCY_UNDER_VOLTAGE     1
#define EMERGENCY_OVER_TEMPERATURE  2
#define EMERGENCY_UNDER_TEMPERATURE 3
#define EMERGENCY_VOLTAGE           0b00000001
#define EMERGENCY_TEMPERATURE       0b00000010

/*#define INTERRUPT_PROTECT(x) {char saved_ipl;SET_AND_SAVE_CPU_IPL(saved_ipl,7);x;RESTORE_CPU_IPL(saved_ipl); } (void) 0;*/

#define UPDATE_CAN_BUFFER_ID can_buffer_id = can_buffer_id < N_TX_BUFFER ? can_buffer_id + 1 : 0

/******************************************************************************
 * Typedefs and structs
 ******************************************************************************/

/*typedef uint16_t ECAN1MSGBUF[ECAN1_MSG_BUF_LENGTH][8];*/

typedef struct {
	uint8_t numberOfCells;
	int16_t Vmax;
	int16_t Vmin;
	int16_t Tmax;
	int16_t Tmin;
	int16_t SOC;
	int16_t current;
	uint16_t board_temperature;
	int16_t voltage[N_CELLS];
	int16_t temperature[N_CELLS];
	int16_t resistance[N_CELLS];
	uint16_t slave_id;
	int16_t cell_id_max_voltage;
	int16_t cell_id_min_voltage;
	int16_t cell_id_max_temp;
	int16_t cell_id_min_temp;
	int16_t abs_max_voltage;
	int16_t abs_min_voltage;
	int16_t abs_max_temp;
	int16_t abs_min_temp;
	bool is_master;
	bool balancing;
	bool requested_balancing;
	bool reply_from_master_request_seen;
	bool expecting_reply;
} SlaveParameters;

typedef struct {
	uint16_t _arbitration;
	uint16_t _control;
	uint16_t _data10;
	uint16_t _data32;
	uint16_t _data54;
	uint16_t _data76;
	uint8_t byte[8];
	uint16_t ide;
	uint16_t srr;
	int16_t sid;
	uint16_t rtr;
	uint16_t rb0;
	uint16_t rb1;
	uint16_t dlc;
	int8_t buffer;
	int16_t frame_type;
	int16_t message_type;
	bool handling_in_process;
} Can_message;

typedef struct {
	int8_t buffer_status;
	int8_t message_type;
	int8_t frame_type;
	int8_t buffer;
	int16_t id;
	int8_t data[8];
	int8_t data_length;
} received_message;

typedef struct {
	received_message *message;
	bool read;
	bool last_message;
} message_queue;

typedef struct {
	// Implementation of http://liionbms.com/php/standards.php
	// ID 620
	char comp[8];
	// ID 621
	char rev[8];
	// ID 622
	int8_t state;
	int16_t timer;
	int8_t flags;
	int8_t dtc1;
	int8_t dtc2;
	// ID 623
	uint16_t voltage;
	uint8_t min_volt;
	uint8_t min_volt_n;
	uint8_t max_volt;
	uint8_t max_volt_n;
	// ID 624
	int16_t current;
	uint16_t charge_limit;
	uint16_t discharge_limit;
	// ID 625
	uint16_t batt_energy_in0;
	uint16_t batt_energy_in1;
	uint16_t batt_energy_out0;
	uint16_t batt_energy_out1;
	// ID 626
	uint8_t soc;
	uint16_t dod;
	uint16_t capacity;
	uint8_t soc2;
	uint8_t soh;
	// ID 627
	int8_t temperature;
	int8_t air_temp;
	int8_t min_temp;
	uint8_t min_temp_n;
	int8_t max_temp;
	uint8_t max_temp_n;
	// ID 628
	uint16_t resistance;
	int8_t min_res;
	uint8_t min_res_no;
	int8_t max_res;
	uint8_t max_res_no;
	// ID 629
	uint16_t ac_line_voltage;
	int16_t ac_line_current;
	uint8_t max_charge_current;
	uint8_t max_discharge_current;
	int16_t ac_line_power;
	// ID 62A
	uint16_t ac_energy_in0;
	uint16_t ac_energy_in1;
	uint16_t ac_energy_out0;
	uint16_t ac_energy_out1;
} STPM; //not all implemented

typedef struct {
	bool is_active;
	int16_t time_last_seen;
} Active_module;

/******************************************************************************
 * Variables
 ******************************************************************************/
volatile extern int16_t system_time_tick;
extern int16_t next_state;
extern int16_t state;
extern int16_t previous_state[PREVIOUS_STATE_LENGTH];
extern int8_t prev_state_ind;
extern int16_t state_to_restore;
extern bool state_changed;
extern int8_t can_buffer_id;
volatile extern int8_t led_pattern;
volatile extern bool t3_fired_off;
volatile extern int16_t interrupt_state;
volatile extern int8_t messages_in_queue;
extern Can_message received_message_queue[50];
void put_message(Can_message);
Can_message get_message(void);
extern Can_message dispatched_message;
extern Can_message message_received;
volatile extern SlaveParameters s_par;
extern STPM stpm_data;

extern char revision[8];
extern char company[8];

extern int16_t batt_volt[N_MODULES][N_CELLS];
extern int16_t batt_temp[N_MODULES][N_CELLS];
extern int16_t batt_curr[N_MODULES];

extern int8_t temperature_table[128];

extern int master_index;
extern FILE *f_output;
#ifdef __cplusplus
}
#endif
extern std::ofstream output;
#endif
