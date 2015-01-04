/*
 * File:   adc.h
 * Author: alrekr
 *
 * Created on November 27, 2014, 12:02 PM
 */

#ifndef ADC_H
#define	ADC_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "structs.h"
#include "slave.h"

#ifdef	__cplusplus
extern "C" {
#endif
	//main process function

	void adc_io_init(void);
	void init_adc(void);
	void adc_enable_interrupt(void);
	void adc_disable_interrupt(void);
#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

