/*
 * File:   slave.c
 * Author: alrekr
 *
 * Created on November 22, 2014, 3:14 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h> //for memcpy
#include <limits.h>
#include "structs.h"
#include "slave.h"
#include "can.h"
#include "common.h"

/*
 *
 */

extern int8_t i;

void init_slave(void) {
	/**************************************************************************
	 * Initialization of slave parameters and variables
	 * Input: None
	 * Output: None
	 **************************************************************************/
	for (i = 0; i < N_CELLS; i++) {
		s_par.voltage[i] = i*20;
		s_par.temperature[i] = i*25%20;
		s_par.resistance[i] = 0;
	}
	s_par.current = 0;
	s_par.Vmax = INT16_MIN;
	s_par.Vmin = INT16_MAX;
	s_par.Tmax = INT16_MIN;
	s_par.Tmin = INT16_MAX;
	s_par.board_temperature = 0;
	s_par.cell_id_max_voltage = -1;
	s_par.cell_id_min_voltage = -1;
	s_par.cell_id_max_temp = -1;
	s_par.cell_id_min_temp = -1;
	s_par.numberOfCells = 8;
	s_par.SOC = 0;
	s_par.abs_max_voltage = 4050; //3.7 V
	s_par.abs_min_voltage = 2736; //2.5 V
	s_par.abs_max_temp = 40;
	s_par.abs_min_temp = 0;
	s_par.balancing = false;
	s_par.requested_balancing = false;
	s_par.reply_from_master_request_seen = false;
	s_par.expecting_reply = false;
	static const int8_t init_temp_table_hack[128] = {
		(118), (112), (107), (102), ( 97), ( 93), ( 88), ( 83),
		( 79), ( 75), ( 71), ( 67), ( 63), ( 59), ( 55), ( 52),
		( 48), ( 45), ( 42), ( 38), ( 35), ( 32), ( 30), ( 27),
		( 24), ( 22), ( 19), ( 17), ( 15), ( 12), ( 10), (  8),
		(  6), (  5), (  3), (  1), ( -1), ( -2), ( -3), ( -5),
		( -6), ( -7), ( -9), (-10), (-11), (-12), (-12), (-13),
		(-14), (-15), (-15), (-16), (-17), (-17), (-17), (-18),
		(-18), (-18), (-19), (-19), (-19), (-19), (-19), (-19),
		(-19), (-19), (-19), (-19), (-19), (-19), (-19), (-18),
		(-18), (-18), (-18), (-17), (-17), (-17), (-16), (-16),
		(-15), (-15), (-15), (-14), (-14), (-13), (-13), (-13),
		(-12), (-12), (-11), (-11), (-11), (-10), (-10), ( -9),
		( -9), ( -9), ( -8), ( -8), ( -8), ( -7), ( -7), ( -7),
		( -7), ( -7), ( -6), ( -6), ( -6), ( -6), ( -6), ( -6),
		( -6), ( -6), ( -6), ( -7), ( -7), ( -7), ( -7), ( -8),
		( -8), ( -9), ( -9), (-10), (-10), (-11), (-12), (-12)
	};
	memcpy(temperature_table, init_temp_table_hack, sizeof(init_temp_table_hack));
}

void balance_cells(void) {
	/**************************************************************************
	 * Balances the cells. All slaves balance when one balance; only the slave
	 * that initiated balancing is able to stop the balancing
	 * Input: None
	 * Output: None
	 **************************************************************************/
	update_max_min();

	if ((s_par.Vmax - s_par.Vmin) > VOLTAGE_DIFF_THRESHOLD) {
		for (i = 0; i < s_par.numberOfCells; i++) {
			if (i != s_par.cell_id_min_voltage) {
				balance_cell(i, true);
			}
		}
	} else {
		for (i = 0; i < s_par.numberOfCells; i++) {
			balance_cell(i, false);
		}
	}
}

void balance_cell(int8_t ident, bool balance_on) {
	/**************************************************************************
	 * Enables or disables balancing of a cell.
	 * Input: Cell identifier and bool indicating if balance or not
	 * Output: None
	 **************************************************************************/
	if (ident < 8) {
		if (balance_on == true) {
			/*LATD = (LATD | (1 << ident));*/
			printf("Enabling balance on Cell no. %d\n", ident);
		} else {
			/*LATD = (LATD & (~(1 << ident)));*/
			printf("Disabling balance on Cell no. %d\n", ident);
		}
	}
}

void update_max_min(void) {
	/**************************************************************************
	 * Updates Vmax and Vmin in s_par
	 * Input: None
	 * Output: None
	 **************************************************************************/
	int i;
	/*DISABLE_INTERRUPTS*/

	s_par.Vmax = INT16_MIN;
	s_par.Vmin = INT16_MAX;
	/*for (i = 0; i < s_par.numberOfCells; i++) {
		if (s_par.voltage[i] < s_par.Vmin) {
			s_par.cell_id_min_voltage = i;
			s_par.Vmin = s_par.voltage[i];
		} else if (s_par.voltage[i] > s_par.Vmax) {
			s_par.cell_id_max_voltage = i;
			s_par.Vmax = s_par.voltage[i];
		}
		if (s_par.temperature[i] < s_par.Tmin) {
			s_par.cell_id_min_temp = i;
			s_par.Tmin = s_par.temperature[i];
		} else if (s_par.temperature[i] > s_par.Tmax) {
			s_par.cell_id_max_temp = i;
			s_par.Tmin = s_par.temperature[i];
		}
	}*/
	/*ENABLE_INTERRUPTS*/
}

void update_abs_max_min_voltage(void) {
	/**************************************************************************
	 * Updates absolute maximum and absolute minimum voltages
	 * Input: None
	 * Output: None
	 **************************************************************************/
	int16_t min_cell_voltage = (dispatched_message.byte[3] & 0x0F) << 8;
	min_cell_voltage |= (dispatched_message.byte[2] & 0xFF);
	int16_t max_cell_voltage = (dispatched_message.byte[1] & 0x0F) << 8;
	max_cell_voltage |= (dispatched_message.byte[0] & 0xFF);
	s_par.abs_max_voltage = max_cell_voltage;
	s_par.abs_min_voltage = min_cell_voltage;
}
