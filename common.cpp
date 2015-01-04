#include <stdint.h>
#include "common.h"
#include <fstream>
#include "stdio.h"

//Can_message dispatched_message;
Can_message message_received;
volatile SlaveParameters s_par;
volatile int8_t messages_in_queue;
volatile int16_t interrupt_state;
int8_t temperature_table[128];
FILE *f_output;
std::ofstream output;
int master_index;
Can_message received_message_queue[50];
char revision[8] = {'v', '0', '0', '0', '0', '0', '0', '1'};
char company[8] = {'M', 'O', 'D', 'U', 'L', 'B', 'M', 'S'};
