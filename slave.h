/*
 * File:   slave.h
 * Author: alrekr
 *
 * Created on November 22, 2014, 3:14 PM
 */
#ifndef SLAVE_H
#define	SLAVE_H

#include <stdbool.h>
#include <stdint.h>
#include "structs.h"
#include "can.h"
#include "slave.h"
#include "common.h"


#ifdef	__cplusplus
extern "C" {
#endif
	void init_slave(void);
	void balance_cells(void);
	void balance_cell(int8_t ident, bool balance_on);
	void disable_interrupts(void);
	void update_max_min(void);
	void update_abs_max_min_voltage(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SLAVE_H */

