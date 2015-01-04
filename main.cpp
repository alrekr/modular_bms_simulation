/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <stdint.h>        /* Includes (u)intX_t definitions                  */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>
#include "adc.h"
#include "can.h"
#include "common.h"
#include "master.h"
#include "slave.h"
#include "state_machine.h"
#include "test_messages.h"

/* New stuff start */
#include <fstream>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
using namespace std;
/* New stuff end */
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void check_for_can_interrupt(void);
void init_fsm_variables(void);
int16_t make_local_state(void);
void Delay_us(int16_t delay);
void my_handler(int s);

int8_t prev_state_ind;
int16_t state;
int16_t next_state;
bool state_changed;
int16_t state_to_restore;
int16_t timestamp;
int16_t local_state;
int8_t from_slave_id;
int8_t i;
int8_t from_slave_data_single;
int8_t from_slave_data_multiple[8];
volatile bool t3_fired_off;
volatile int16_t system_time_tick;
volatile bool time_overflowed;
/* New stuff start */
time_t rawtime;
struct tm *timeinfo;
char buffer[80];
extern int master_index;
extern Can_message received_message_queue[50];
int8_t read_index = 0, write_index = 0;
bool do_not_fire_again = false;
bool srsly_dont_fire_again = false;
/* New stuff end */

int main(void) {
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);

	/*ConfigureOscillator();*/
	printf("Started.\n");
	master_index = 0;
	s_par.is_master = false;
	init_fsm_variables();
	while (1) {
		printf("While loop.\n");
#ifdef MASTER
		if(master_index > 15) {
#else
		if(master_index > 20) {
#endif
			fclose(f_output);
			return 1;
		}

		master_index++;
		//handle_message();
		printf("State is %d\n", state);
		switch (state) {
		case FSM_INIT:

			/* Initialize IO ports and peripherals */

			/*init_gpio();*/
			/*adc_io_init();*/
			init_slave();
			init_adc();
			init_can();
			set_up_test_messages();
			/*while (s_par.voltage[0] == 0); //wait for first ADC value
		{
			int16_t delay = s_par.voltage[0] * 9; //max(2**12*9)=0x240000,max(2**16)=0x400000 - up to 3.7 seconds delay
			Delay_us(delay);
		}
			request_master_status(&timestamp);
		{
			int8_t wd, wds;
			bool breakout = false;
			for (wds = 0; wds < 1221; wds++) {
				for (wd = 0; wd < 32767; wd++) { //wait for maximum 40008507 steps (two seconds or so on PIC24E- ample time for the master to respond!
					if (handle_message() == true || (s_par.reply_from_master_request_seen == true && s_par.expecting_reply == true)) {
						init_master();
						breakout = true;
						s_par.expecting_reply = false;
						break;
					}
				}
				if (breakout == true) {
					break;
				}
			}

			if (wds == 1221 && wd == 32767) {
				s_par.is_master = false;
				init_slave();
			}
		}*/
			/* New stuff start */
#ifdef MASTER
			init_master();
			//commands.open("/home/alrekr/Dropbox/Workspaces/qt/modular_bms/messages_for_master.txt");
			//output.open("/home/alrekr/Dropbox/Workspaces/qt/modular_bms/output_from_master.txt");
			f_output = fopen("/home/alrekr/Dropbox/Skole/Bachelor/rapport/data/output-from-master.txt", "w");
#else
			//commands.open("/home/alrekr/Dropbox/Workspaces/qt/modular_bms/messages_for_slave.txt");
			f_output = fopen("/home/alrekr/Dropbox/Skole/Bachelor/rapport/data/output-from-slave.txt", "w");

#endif
			//{
			//	std::string dump;
			//	getline(commands, dump);
			//}
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			strftime(buffer, 80, "%F %T", timeinfo);
			fprintf(f_output, "================================================================================\nFile opened by new process at time %s\n", buffer);
#ifdef MASTER
			for (i = 0; i < 9; i++) {
#else
			for (i = 0; i < 17; i++) {
#endif
				put_message(test_msg[i]);
			}
			//message_received = get_message();
			/* New stuff end */
			// ask for master status
			if (s_par.is_master == false){
				int16_t ts = 0;
				request_master_status(&ts);
			}
			update_state(FSM_CHARGING);
			break;
		case FSM_SEND_VOLTAGE_DATA:
			printf("In state FSM_SEND_VOLTAGE_DATA\n");
			fprintf(f_output, "In state FSM_SEND_VOLTAGE_DATA\n");
			write_voltage_data();
			state_to_restore = update_state(FSM_SEND_TEMPERATURE_DATA);
			break;
		case FSM_SEND_TEMPERATURE_DATA:
			printf("In state FSM_SEND_TEMPERATURE_DATA\n");
			fprintf(f_output, "In state FSM_SEND_TEMPERATURE_DATA\n");
			write_temperature_data();
			restore_state(state_to_restore);
		case FSM_CHARGING:
			printf("In state FSM_CHARGING\n");
			fprintf(f_output, "In state FSM_CHARGING\n");
			/*update_max_min();
			if (s_par.balancing == true) {
				balance_cells();
				if ((s_par.Vmax - s_par.Vmin) <= VOLTAGE_DIFF_THRESHOLD && s_par.requested_balancing == true) {
					slave_request_balancing(false);
					s_par.requested_balancing = false;
				}
			} else if ((s_par.abs_max_voltage - s_par.Vmax) <= VOLTAGE_DIFF_THRESHOLD) {
				slave_request_balancing(true);
				s_par.requested_balancing = true;
			}
			if (s_par.Tmax <= s_par.abs_max_temp) {
				send_emergency_stop(EMERGENCY_TEMPERATURE);
			} else if (s_par.Tmin >= s_par.abs_min_temp) {
				send_emergency_stop(EMERGENCY_TEMPERATURE);
			} else if (s_par.Vmax >= s_par.abs_max_voltage) {
				send_emergency_stop(EMERGENCY_VOLTAGE);
			} else if (s_par.Vmin >= s_par.abs_min_voltage) {
				send_emergency_stop(EMERGENCY_VOLTAGE);
			}*/
			break;
		case FSM_DISCHARGING:
			printf("In state FSM_DISCHARGING\n");
			fprintf(f_output, "In state FSM_DISCHARGING\n");
			update_max_min();
			/*if (s_par.Vmin <= s_par.abs_min_voltage) {
				send_emergency_stop(EMERGENCY_VOLTAGE);
			} else if (s_par.Vmax >= s_par.abs_max_voltage) {
				send_emergency_stop(EMERGENCY_VOLTAGE);
			} else if (s_par.Tmax <= s_par.abs_max_temp) {
				send_emergency_stop(EMERGENCY_TEMPERATURE);
			} else if (s_par.Tmin >= s_par.abs_min_temp) {
				send_emergency_stop(EMERGENCY_TEMPERATURE);
			}*/
			break;
		case FSM_HANDLE_MESSAGE: {
			printf("In state FSM_HANDLE_MESSAGE\n");
			fprintf(f_output, "In state FSM_HANDLE_MESSAGE\n");
			if (dispatched_message.handling_in_process == false) {
				dispatched_message.handling_in_process = true;
				printf("sid = %x\n", dispatched_message.sid);
				if (dispatched_message.sid == EMERGENCY_STOP_ID) {
					//send emergency stop to external unit. Not implemented.
				}
				local_state = make_local_state();
				from_slave_id = dispatched_message.sid & ~CAN_MSG_MASK;
				printf("local_state = %d\tfrom_slave_id = %x\n", local_state, from_slave_id);
				fprintf(f_output, "local_state = %d\tfrom_slave_id = %x\n", local_state, from_slave_id);
				switch (local_state) {
				case FSM_LOC_REQUEST_MASTER:
					printf("In state FSM_LOC_REQUEST_MASTER\n");
					fprintf(f_output, "In state FSM_LOC_REQUEST_MASTER\n");
					send_slave_id(assign_slave_id());
					break;
				case FSM_LOC_REQ_VOLTAGE:
					printf("In state FSM_LOC_REQ_VOLTAGE\n");
					fprintf(f_output, "In state FSM_LOC_REQ_VOLTAGE\n");
					write_voltage_data();
					break;
				case FSM_LOC_REQ_TEMPERATURE:
					printf("In state FSM_LOC_REQ_TEMPERATURE\n");
					fprintf(f_output, "In state FSM_LOC_REQ_TEMPERATURE\n");
					write_temperature_data();
					break;
				case FSM_LOC_REQ_CURRENT:
					printf("In state FSM_LOC_CURRENT\n");
					fprintf(f_output, "In state FSM_LOC_CURRENT\n");
					//write_current_data() //not implemented
					break;
				case FSM_LOC_REPLY_VOLTAGE:
					printf("In state FSM_LOC_REPLY_VOLTAGE\n");
					fprintf(f_output, "In state FSM_LOC_REPLY_VOLTAGE\n");
					memcpy(from_slave_data_multiple, dispatched_message.byte, sizeof (dispatched_message.byte));
					fprintf(f_output, "Storing applicable voltage data in 2D array.\n");
					for (i = 0; i < N_CELLS; i++) {
						batt_volt[from_slave_id][i] = from_slave_data_multiple[i];
					}
					module_watchdog(dispatched_message.sid);
					break;
				case FSM_LOC_REPLY_TEMPERATURE:
					printf("In state FSM_LOC_REPLY_TEMPERATURE\n");
					fprintf(f_output, "In state FSM_LOC_REPLY_TEMPERATURE\n");
					memcpy(from_slave_data_multiple, dispatched_message.byte, sizeof (dispatched_message.byte));
					fprintf(f_output, "Storing applicable temperature data in 2D array.\n");
					for (i = 0; i < N_CELLS; i++) {
						batt_temp[from_slave_id][i] = from_slave_data_multiple[i];
					}
					module_watchdog(dispatched_message.sid);
					break;
				case FSM_LOC_REPLY_CURRENT:
					printf("In state FSM_LOC_REPLY_CURRENT\n");
					fprintf(f_output, "In state FSM_LOC_REPLY_CURRENT\n");
					from_slave_data_single = dispatched_message.byte[0];
					batt_curr[from_slave_id] = from_slave_data_single;
					module_watchdog(dispatched_message.sid);
					break;
				case FSM_LOC_REQ_BALANCING:
					printf("In state FSM_LOC_REQ_BALANCING\n");
					fprintf(f_output, "In state FSM_LOC_REQ_BALANCING\n");
					write_charge_status(CAN_SET_BALANCING);
					slave_that_requested_balancing = from_slave_id;
					break;
				case FSM_LOC_BALANCING_DONE:
					printf("In state FSM_LOC_BALANCING_DONE\n");
					fprintf(f_output, "In state FSM_LOC_BALANCING_DONE\n");
					if (from_slave_id == slave_that_requested_balancing) {
						write_charge_status(CAN_SET_CHARGING);
					}
					break;
				case FSM_LOC_REPLY_FROM_MASTER:
					printf("In state FSM_LOC_REPLY_FROM_MASTER\t\tID is %d\n", from_slave_id);
					fprintf(f_output, "In state FSM_LOC_REPLY_FROM_MASTER\t\tID is %d\n", from_slave_id);
					s_par.slave_id = from_slave_id;
					break;
				case FSM_LOC_CHARGE_STATUS:
					printf("In state FSM_LOC_CHARGE_STATUS\n");
					fprintf(f_output, "In state FSM_LOC_CHARGE_STATUS\n");
					update_abs_max_min_voltage();
					update_state(FSM_CHARGING);
					break;
				case FSM_LOC_DISCHARGE_STATUS:
					printf("In state FSM_LOC_DISCHARGE_STATUS\n");
					fprintf(f_output, "In state FSM_LOC_DISCHARGE_STATUS\n");
					update_abs_max_min_voltage();
					update_state(FSM_DISCHARGING);
					break;
				case FSM_LOC_BALANCING_STATUS:
					printf("In state FSM_LOC_BALANCING_STATUS\n");
					fprintf(f_output, "In state FSM_LOC_BALANCING_STATUS\n");
					update_abs_max_min_voltage();
					update_state(FSM_CHARGING); //charging and balancing is in one state
					break;
				case FSM_LOC_INVALID_STATE:
					break;
				}
				dispatched_message.handling_in_process = false;
			}
			update_state(FSM_CHARGING);
		}
			break;
		}
		if (t3_fired_off == true && s_par.is_master == true && (do_not_fire_again == false || srsly_dont_fire_again == false)) {
			prepare_stpm();
			write_stpm_data();
			t3_fired_off = false;
			if(do_not_fire_again == true) {
				srsly_dont_fire_again = true;
			}
			do_not_fire_again = true;
		}
		/*if (state_to_restore != FSM_INVALID_STATE) {
			restore_state(state_to_restore);
		}*/
		if (read_index != write_index && master_index > 1) {
			printf("Checking for can interrupts.\n");
			check_for_can_interrupt();
		} else {
			t3_fired_off = true;
		}
		update_max_min();
		for(i = 0; i < N_WATCHDOG_MODULES; i++) {
			module_watchdog(i);
		}
		fprintf(f_output, "--------------------\n");
		//printf("State at end is %d\n--------------------\n", state);
		//fprintf(f_output, "State at end is %d\n--------------------\n", state);
	}
}

void check_for_can_interrupt(void) {
	/**************************************************************************
	 * Actual interrupt routine for CAN interrupts. Moved to main() to allow
	 * for calling subroutines.
	 * Input: None
	 * Output: None
	 **************************************************************************/
	/*if (C1RXFUL1 != 0x0000 && t3_fired_off == false) {
		if (message_received.handling_in_process == false) {
			message_received.handling_in_process = true;
			IFS2 &= ~0x04;
			C1INTFbits.RBIF = 0;
			C1INTEbits.RBIE = 0;
			int buffer;
			switch (C1RXFUL1) {
				case 0x0001:
					buffer = 0;
					C1RXFUL1bits.RXFUL0 = 0;
					break;
				case 0x0002:
					buffer = 1;
					C1RXFUL1bits.RXFUL1 = 0;
					break;
				case 0x0004:
					buffer = 2;
					C1RXFUL1bits.RXFUL2 = 0;
					break;
				case 0x0008:
					buffer = 3;
					C1RXFUL1bits.RXFUL3 = 0;
					break;
				case 0x0010:
					buffer = 4;
					C1RXFUL1bits.RXFUL4 = 0;
					break;
				case 0x0020:
					buffer = 5;
					C1RXFUL1bits.RXFUL5 = 0;
					break;
				case 0x0040:
					buffer = 6;
					C1RXFUL1bits.RXFUL6 = 0;
					break;
				case 0x0080:
					buffer = 7;
					C1RXFUL1bits.RXFUL7 = 0;
					break;
				case 0x0100:
					buffer = 8;
					C1RXFUL1bits.RXFUL8 = 0;
					break;
				case 0x0200:
					buffer = 9;
					C1RXFUL1bits.RXFUL9 = 0;
					break;
				case 0x0400:
					buffer = 10;
					C1RXFUL1bits.RXFUL10 = 0;
					break;
				case 0x0800:
					buffer = 11;
					C1RXFUL1bits.RXFUL11 = 0;
					break;
				case 0x1000:
					buffer = 12;
					C1RXFUL1bits.RXFUL12 = 0;
					break;
				case 0x2000:
					buffer = 13;
					C1RXFUL1bits.RXFUL13 = 0;
					break;
				case 0x4000:
					buffer = 14;
					C1RXFUL1bits.RXFUL14 = 0;
					break;
				case 0x8000:
					buffer = 15;
					C1RXFUL1bits.RXFUL15 = 0;
					break;
				default:*/
	/* In case more than one buffer is full. Such a situation
	 * will solved, assuming MCU time is much faster than IO
	 * Can time */
	/*if (C1RXFUL1bits.RXFUL0 == 1) {
						buffer = 0;
						C1RXFUL1bits.RXFUL0 = 0;
					} else if (C1RXFUL1bits.RXFUL1 == 1) {
						buffer = 1;
						C1RXFUL1bits.RXFUL1 = 0;
					} else if (C1RXFUL1bits.RXFUL2 == 1) {
						buffer = 2;
						C1RXFUL1bits.RXFUL2 = 0;
					} else if (C1RXFUL1bits.RXFUL3 == 1) {
						buffer = 3;
						C1RXFUL1bits.RXFUL3 = 0;
					} else if (C1RXFUL1bits.RXFUL4 == 1) {
						buffer = 4;
						C1RXFUL1bits.RXFUL4 = 0;
					} else if (C1RXFUL1bits.RXFUL5 == 1) {
						buffer = 5;
						C1RXFUL1bits.RXFUL5 = 0;
					} else if (C1RXFUL1bits.RXFUL6 == 1) {
						buffer = 6;
						C1RXFUL1bits.RXFUL6 = 0;
					} else if (C1RXFUL1bits.RXFUL7 == 1) {
						buffer = 7;
						C1RXFUL1bits.RXFUL7 = 0;
					} else if (C1RXFUL1bits.RXFUL8 == 1) {
						buffer = 8;
						C1RXFUL1bits.RXFUL8 = 0;
					} else if (C1RXFUL1bits.RXFUL9 == 1) {
						buffer = 9;
						C1RXFUL1bits.RXFUL9 = 0;
					} else if (C1RXFUL1bits.RXFUL10 == 1) {
						buffer = 10;
						C1RXFUL1bits.RXFUL10 = 0;
					} else if (C1RXFUL1bits.RXFUL11 == 1) {
						buffer = 11;
						C1RXFUL1bits.RXFUL11 = 0;
					} else if (C1RXFUL1bits.RXFUL12 == 1) {
						buffer = 12;
						C1RXFUL1bits.RXFUL12 = 0;
					} else if (C1RXFUL1bits.RXFUL13 == 1) {
						buffer = 13;
						C1RXFUL1bits.RXFUL13 = 0;
					} else if (C1RXFUL1bits.RXFUL14 == 1) {
						buffer = 14;
						C1RXFUL1bits.RXFUL14 = 0;
					} else if (C1RXFUL1bits.RXFUL15 == 1) {
						buffer = 15;
						C1RXFUL1bits.RXFUL15 = 0;
					}
					break;
			}
			message_received.buffer = buffer;
			message_received.handling_in_process = false;
			while (handle_message() != true); //make sure message is handled
			DISABLE_INTERRUPTS
			if (interrupt_state != FSM_INVALID_STATE) {
				state_to_restore = update_state(interrupt_state);
				interrupt_state = FSM_INVALID_STATE;
			}
			ENABLE_INTERRUPTS
		}
	}*/

	message_received = get_message();
	handle_message();
	update_state(FSM_HANDLE_MESSAGE);
}

int16_t make_local_state(void) {
	/**************************************************************************
	 * Defines new local state from dispatched message id. Attentive to master.
	 * Input: None
	 * Output: New local state
	 **************************************************************************/
	int16_t msg_id = dispatched_message.sid & ~CAN_ID_MASK;
	int16_t sender_id = dispatched_message.sid & ~CAN_MSG_MASK;
	printf("msg_id = %x\tsender_id = %d\n", msg_id, sender_id);
	fprintf(f_output, "msg_id = %x\tsender_id = %d\n", msg_id, sender_id);
	int16_t ns = FSM_LOC_INVALID_STATE;
	if (s_par.is_master == true) {
		switch (msg_id) {
		case MASTER_REQUEST_ID:
			ns = FSM_LOC_REQUEST_MASTER;
			break;
		case BALANCE_ID:
			ns = FSM_LOC_REQ_BALANCING;
			break;
		case BALANCE_DONE_ID:
			ns = FSM_LOC_BALANCING_DONE;
			break;
		case VOLTAGE_ID:
			ns = FSM_LOC_REPLY_VOLTAGE;
			break;
		case TEMPERATURE_ID:
			ns = FSM_LOC_REPLY_TEMPERATURE;
			break;
		case CURRENT_ID:
			ns = FSM_LOC_REPLY_CURRENT;
			break;
		}
	} else  {
		switch (msg_id) {
		case MASTER_REPLY_ID:
			ns = FSM_LOC_REPLY_FROM_MASTER;
			break;
		case CHARGE_STATUS_ID:
			if (sender_id == MASTER_ID) {
				ns = FSM_LOC_CHARGE_STATUS;
			}
			break;
		case VOLTAGE_ID:
			if (sender_id == MASTER_ID) {
				ns = FSM_LOC_REQ_VOLTAGE;
			}
			break;
		case TEMPERATURE_ID:
			if (sender_id == MASTER_ID) {
				ns = FSM_LOC_REQ_TEMPERATURE;
			}
			break;
		case CURRENT_ID:
			if (sender_id == MASTER_ID) {
				ns = FSM_LOC_REQ_CURRENT;
			}
			break;
		}
	}
	return ns;
}

void Delay_us(int16_t delay) {
	/*some function*/
	while(delay < 0) {
		--delay;
	}
}

void put_message(Can_message to_put) {
	/*************************************************************************
	 * Puts message in queue.
	 * Input: Messages
	 * Output: None
	 *************************************************************************/
	received_message_queue[write_index] = to_put;
	write_index++;
	if (write_index == 50) {
		write_index = 0;
	}
}

Can_message get_message(void) {
	/*************************************************************************
	 * Gets oldest message from queue
	 * Input: None
	 * Output: Oldest message in queue
	 *************************************************************************/
	Can_message to_return;

	to_return = received_message_queue[read_index];
	read_index++;
	if (read_index == 48) {
		read_index = 0;
	}

	return to_return;
}

void my_handler(int s){
	printf("Caught signal %d\n",s);
	fclose(f_output);
	exit(1);
}
