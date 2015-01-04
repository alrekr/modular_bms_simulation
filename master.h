/*
 * File:   master.h
 * Author: alrekr
 *
 * Created on November 22, 2014, 3:14 PM
 */

#ifndef MASTER_H
#define	MASTER_H

#include <stdbool.h>
#include <stdint.h>
#include "common.h"
#include "can.h"
#include "slave.h"


#ifdef	__cplusplus
extern "C" {
#endif
extern Active_module module[N_WATCHDOG_MODULES];
extern bool assigned_slave_ids[N_WATCHDOG_MODULES];
volatile extern bool time_overflowed;
extern int16_t batt_volt[N_MODULES][N_CELLS];
extern int16_t batt_temp[N_MODULES][N_CELLS];
extern int16_t batt_curr[N_MODULES];
extern STPM stpm_data;
extern int8_t i_master, j_master;
extern int8_t id;
extern int8_t slave_id;
extern int16_t high_volt;
extern int16_t low_volt;
extern int16_t high_temp;
extern int16_t low_temp;
extern int8_t slave_that_requested_balancing;

void init_master(void);
void init_stpm_data(void);
void reset_stpm_data(void);
int8_t assign_slave_id(void);
void module_watchdog(int16_t message_id);
void prepare_stpm(void);
bool slave_is_active(int16_t message_id);

#ifdef	__cplusplus
}
#endif

#endif	/* MASTER_H */

