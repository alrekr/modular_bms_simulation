/*
 * File:   adc.c
 * Author: alrekr
 *
 * Created on November 27, 2014, 12:05 PM
 */

#include "adc.h"
#include "slave.h"
#include "common.h"

volatile extern SlaveParameters s_par;

void adc_io_init() {
	/**************************************************************************
	 * Initialises ADC IO
	 * Input: None
	 * Output: None
	 **************************************************************************/
	/*TRISBbits.TRISB0 = 1; // VREF
	ANSELBbits.ANSB0 = 1;
	TRISBbits.TRISB1 = 1; // GNDREF
	ANSELBbits.ANSB1 = 1;

	TRISBbits.TRISB2 = 1; // Cell 1
	ANSELBbits.ANSB2 = 1;
	TRISBbits.TRISB3 = 1; // Cell 2
	ANSELBbits.ANSB3 = 1;
	TRISBbits.TRISB4 = 1; // Cell 3
	ANSELBbits.ANSB4 = 1;
	TRISBbits.TRISB5 = 1; // Cell 4
	ANSELBbits.ANSB5 = 1;

	TRISBbits.TRISB8 = 1; // Cell 5
	ANSELBbits.ANSB8 = 1;
	TRISBbits.TRISB9 = 1; // Cell 6
	ANSELBbits.ANSB9 = 1;
	TRISBbits.TRISB10 = 1; // Cell 7
	ANSELBbits.ANSB10 = 1;
	TRISBbits.TRISB11 = 1; // Cell 8
	ANSELBbits.ANSB11 = 1;

	TRISBbits.TRISB12 = 1; // Temperature Mux
	ANSELBbits.ANSB12 = 1;

	TRISBbits.TRISB13 = 1; // Board Temp
	ANSELBbits.ANSB13 = 1;*/
}

void init_adc() {
	/**************************************************************************
	 * Initialises ADC
	 * Input: None
	 * Output: None
	 **************************************************************************/
	s_par.numberOfCells = 8;
	/*adc_io_init();
	//Disable storing to DMA, use ADCxBUF0 - ADCxBUFf
	AD1CON4bits.ADDMAEN = 0;
	// Data Output Format: Signed Fraction (Q15 format)
	AD1CON1bits.FORM = 0;
	// Sample Clock Source: User starts conversion
	AD1CON1bits.SSRC = 0;
	// ADC Sample Control: Sampling begins immediately after conversion
	AD1CON1bits.ASAM = 1;
	//12 bit sampling
	AD1CON1bits.AD12B = 1;
	AD1CSSL = 0x0000;
	AD1CSSH = 0x0000;
	AD1CSSLbits.CSS2 = 1;
	AD1CSSLbits.CSS3 = 1;
	AD1CSSLbits.CSS4 = 1;
	AD1CSSLbits.CSS5 = 1;
	AD1CSSLbits.CSS8 = 1;
	AD1CSSLbits.CSS9 = 1;
	AD1CSSLbits.CSS10 = 1;
	AD1CSSLbits.CSS11 = 1;
	AD1CSSLbits.CSS12 = 1;
	AD1CSSLbits.CSS13 = 1;
	AD1CSSHbits.CSS16 = 1;
	// P = 01 => external Vref+
	// N = 0  => AVss
	AD1CON2bits.VCFG = 0b001;

	//AD1CON2 Register
	// Generate interrupt every 16 sample/conversion
	AD1CON2bits.SMPI = 0x10;
	// Buffer configured as one 16-word buffers
	AD1CON2bits.BUFM = 0;
	AD1CON2bits.CSCNA = 1;
	//AD1CON3 Register
	// ADC Clock is derived from Systems Clock
	AD1CON3bits.ADRC = 0;

	// ADC Conversion Clock Tad=Tcy*(ADCS+1)=(1/40M)*2*32 = 8us (125Khz)
	// ADC Conversion Time for 10-bit Tc=12*Tab = 96us
	AD1CON3bits.ADCS = 31;
	adc_enable_interrupt();
	AD1CON1bits.ADON = 1; // turn ADC ON*/
}

void adc_enable_interrupt() {
	/**************************************************************************
	 * Enables interrupts for ADC
	 * Input: None
	 * Output: None
	 **************************************************************************/
	/*IEC0bits.AD1IE = 1; // Enable A/D interrupt
	IFS0bits.AD1IF = 0; // Clear interrupt flag*/
}

void adc_disable_interrupt() {
	/**************************************************************************
	 * Disables interrupts for ADC
	 * Input: None
	 * Output: None
	 **************************************************************************/
	/*IEC0bits.AD1IE = 0; // Disable A/D interrupt
	IFS0bits.AD1IF = 0; // Clear interrupt flag*/
}
