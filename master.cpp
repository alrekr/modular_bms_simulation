/*
 * File:   master.c
 * Author: alrekr
 *
 * Created on November 22, 2014, 3:14 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "master.h"
#include "can.h"
#include "slave.h"
#include "common.h"

Active_module module[N_WATCHDOG_MODULES];
bool assigned_slave_ids[N_WATCHDOG_MODULES];
int16_t batt_volt[N_MODULES][N_CELLS];
int16_t batt_temp[N_MODULES][N_CELLS];
int16_t batt_curr[N_MODULES];
int8_t i_master, j_master;
STPM stpm_data;
//const char company[] = STPM_COMPANY;
//const char revision[] = STPM_REVISION;
int8_t id = -1;
int8_t slave_id = CAN_ID_MASK;
int16_t high_volt;
int16_t low_volt;
int16_t high_temp;
int16_t low_temp;
int8_t slave_that_requested_balancing;

void init_master() {
	/**************************************************************************
	 * Initialization of all variables used by master.c/h
	 * Input: is_master, boolean value indicating if module is master
	 * Output: Yellow LED is turned on; none in program
	 **************************************************************************/
	/*LED_YELLOW_W = 0x01;*/
	t3_fired_off = false;
	s_par.slave_id = 0;
	s_par.is_master = true;
	init_slave();
	assigned_slave_ids[0] = true;
	module[0].is_active = true;
	module[0].time_last_seen = 0;


	for (i_master = 1; i_master < N_WATCHDOG_MODULES; i_master++) {
		assigned_slave_ids[i_master] = false;
		module[i_master].is_active = false;
		module[i_master].time_last_seen = 0;
	}

	for (i_master = 0; i_master < N_MODULES; i_master++) {
		for (j_master = 0; j_master < N_CELLS; j_master++) {
			batt_volt[i_master][j_master] = 0;
			batt_temp[i_master][j_master] = 0;
		}
		batt_curr[i_master] = 0;
	}

	system_time_tick = 0;
	init_stpm_data();
	t3_fired_off = false;
	/*configure_timer1();
	configure_timer2_3();
	configure_timer4();*/

}

void init_stpm_data(void) {
	/**************************************************************************
	 * Initialize stpm_data.
	 * Input: None
	 * Output: None
	 **************************************************************************/
	stpm_data.comp[0] = company[0];
	stpm_data.comp[1] = company[1];
	stpm_data.comp[2] = company[2];
	stpm_data.comp[3] = company[3];
	stpm_data.comp[4] = company[4];
	stpm_data.comp[5] = company[5];
	stpm_data.comp[6] = company[6];
	stpm_data.comp[7] = company[7];

	stpm_data.rev[0] = revision[0];
	stpm_data.rev[1] = revision[1];
	stpm_data.rev[2] = revision[2];
	stpm_data.rev[3] = revision[3];
	stpm_data.rev[4] = revision[4];
	stpm_data.rev[5] = revision[5];
	stpm_data.rev[6] = revision[6];
	stpm_data.rev[7] = revision[7];

	reset_stpm_data();
}

void reset_stpm_data(void) {
	/**************************************************************************
	 * Sets all values except company and revision to invalid values.
	 * Input: None
	 * Output: None
	 **************************************************************************/
	stpm_data.state = '-';
	stpm_data.timer = 0x2d2d;
	stpm_data.flags = '-';
	stpm_data.dtc1 = '-';
	stpm_data.dtc2 = '-';

	stpm_data.voltage = 0x2d2d;
	stpm_data.min_volt = '-';
	stpm_data.min_volt_n = '-';
	stpm_data.max_volt = '-';
	stpm_data.max_volt_n = '-';

	stpm_data.current = 0x2d2d;
	stpm_data.charge_limit = 0x2d2d;
	stpm_data.discharge_limit = 0x2d2d;

	stpm_data.batt_energy_in0 = 0x2d2d;
	stpm_data.batt_energy_in1 = 0x2d2d;
	stpm_data.batt_energy_out0 = 0x2d2d;
	stpm_data.batt_energy_out1 = 0x2d2d;

	stpm_data.soc = '-';
	stpm_data.dod = 0x2d2d;
	stpm_data.capacity = 0x2d2d;
	stpm_data.soc2 = '-';
	stpm_data.soh = '-';

	stpm_data.temperature = '-';
	stpm_data.air_temp = '-';
	stpm_data.min_temp = '-';
	stpm_data.min_temp_n = '-';
	stpm_data.max_temp = '-';
	stpm_data.max_temp_n = '-';

	stpm_data.resistance = 0x2d2d;
	stpm_data.min_res = '-';
	stpm_data.min_res_no = '-';
	stpm_data.max_res = '-';
	stpm_data.max_res_no = '-';

	stpm_data.ac_line_voltage = 0x2d2d;
	stpm_data.ac_line_current = 0x2d2d;
	stpm_data.max_charge_current = '-';
	stpm_data.max_discharge_current = '-';
	stpm_data.ac_line_power = 0x2d2d;

	stpm_data.ac_energy_in0 = 0x2d2d;
	stpm_data.ac_energy_in1 = 0x2d2d;
	stpm_data.ac_energy_out0 = 0x2d2d;
	stpm_data.ac_energy_out1 = 0x2d2d;
}

int8_t assign_slave_id(void) {
	/**************************************************************************
	 * Assigns slave IDs to new modules signing in.
	 * Input: None
	 * Output: Signed int ID. If less than 0, an error has occurred.
	 *************************************************************************/
	for (i_master = 1; i_master < N_WATCHDOG_MODULES; i_master++) { //id 0 is always the master
		if (assigned_slave_ids[i_master] == false) {
			assigned_slave_ids[i_master] = true;
			module[i_master].is_active = true;
			id = i_master;
			break;
		}
	}

	return id;
}

void module_watchdog(int16_t message_id) {
	/**************************************************************************
	 * Keeps track on all modules, sets module as offline after 2 seconds
	 * inactivity. Uses boolean array _active_modules[SLAVE ID] to keep track.
	 * This routine should be called every time a message about voltage or
	 * temperature is read on the CAN bus.
	 * Input: message_id, taken from a voltage or temperature message
	 * Output: None
	 **************************************************************************/
	slave_id = message_id & CAN_ID_MASK;

	if (time_overflowed == true) { // Time overflowed to 0.
		for (i_master = 0; i_master < N_WATCHDOG_MODULES; i_master++) {
			if (module[i_master].is_active == true) {
				//this solution might give a problem if the system is running for more than
				//**15-1 seconds (9 hours), but then only for two seconds until another 9 hours has passed.
				module[slave_id].time_last_seen = module[slave_id].time_last_seen - INT16_MAX;
			}
		}
		time_overflowed = false;
	}

	if ((module[slave_id].time_last_seen - 2) < system_time_tick && module[slave_id].is_active == true) { //if last seen more than 2 seconds ago and is still active
		module[slave_id].is_active = false;
	} else {
		module[slave_id].time_last_seen = system_time_tick;
	}
}

void prepare_stpm(void) {
	/**************************************************************************
	 * Sends STPM
	 * Input: None
	 * Output: None
	 **************************************************************************/
	//int16_t curr = 0; //not implemented
	high_volt = 0;
	low_volt = INT16_MAX;
	high_temp = 0;
	low_temp = INT16_MAX;
	stpm_data.voltage = 0;
	stpm_data.temperature = 0;
	for(i_master = 0; i_master < N_CELLS; i_master++) {
		if (s_par.voltage[i_master] > high_volt) {
			stpm_data.max_volt = s_par.voltage[i_master];
			stpm_data.max_volt_n = i_master;
			high_volt = s_par.voltage[i_master];
		} else if (s_par.voltage[i_master] < low_volt) {
			stpm_data.min_volt = s_par.voltage[i_master];
			stpm_data.min_volt_n = i_master;
			low_volt = s_par.voltage[i_master];
		}
		if (s_par.temperature[i_master] > high_temp) {
			stpm_data.max_temp = s_par.temperature[i_master];
			stpm_data.max_temp_n = i_master;
			s_par.temperature[i_master];
		} else if (s_par.temperature[i_master] < low_temp) {
			stpm_data.min_temp = s_par.temperature[i_master];
			stpm_data.min_temp_n = i_master;
			low_temp = s_par.temperature[i_master];
		}
		stpm_data.voltage += s_par.voltage[i_master];
		stpm_data.temperature += s_par.temperature[i_master];
	}
/*
	for (i_master = 0; i_master < N_MODULES; i_master++) {
		for (j_master = 0; j_master < N_CELLS; j_master++) {
			if (batt_volt[i_master][j_master] > high_volt) {
				stpm_data.max_volt = batt_volt[i_master][j_master];
				stpm_data.max_volt_n = j_master;
				stpm_data.max_volt_n |= i_master << 3;
				high_volt = batt_volt[i_master][j_master];
			} else if (batt_volt[i_master][j_master] < low_volt) {
				stpm_data.min_volt = batt_volt[i_master][j_master];
				stpm_data.min_volt_n = j_master;
				stpm_data.min_volt_n |= i_master << 3;
				low_volt = batt_volt[i_master][j_master];
			}
			if (batt_temp[i_master][j_master] > high_temp) {
				stpm_data.max_temp = batt_temp[i_master][j_master];
				stpm_data.max_temp_n = j_master;
				stpm_data.max_temp_n |= i_master << 3;
				high_temp = batt_temp[i_master][j_master];
			} else if (batt_temp[i_master][j_master] < low_temp) {
				stpm_data.min_temp = batt_temp[i_master][j_master];
				stpm_data.min_temp_n = j_master;
				stpm_data.min_temp_n |= i_master << 3;
				low_temp = batt_temp[i_master][j_master];
			}
		}
		//curr += batt_curr[i]; //battery current is not implemented on test board
	}*/
	//stpm_data.current = _round((double)curr / N_MODULES);
	stpm_data.air_temp = 31;
}

bool slave_is_active(int16_t message_id) {
	/**************************************************************************
	 * Returns whether a slave module is active or not
	 * Input: Slave ID
	 * Output: True if slave is active; false if slave is inactive
	 **************************************************************************/
	slave_id = message_id & CAN_ID_MASK;
	return module[slave_id].is_active;
}
