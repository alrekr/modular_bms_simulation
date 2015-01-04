#include <stdint.h>
#include "common.h"
#include "state_machine.h"

int16_t previous_state[PREVIOUS_STATE_LENGTH];
extern int8_t i;

void init_fsm_variables(void) {
	/*************************************************************************
	 * Initalizes state machine variables
	 * Input: None
	 * Output: None
	 *************************************************************************/
	for (i = 0; i < PREVIOUS_STATE_LENGTH; i++) {
		previous_state[i] = FSM_INIT;
	}
	prev_state_ind = 0;
	state = FSM_INIT;
	next_state = FSM_INIT;
	state_changed = false;
	state_to_restore = FSM_INVALID_STATE;
}


int16_t update_state(int16_t new_state) {
	/**************************************************************************
	 * Updates state to next state, saves previous state
	 * Input: Which new state to set.
	 * Output: Which index the previous state was.
	 **************************************************************************/
	int16_t return_state;
	previous_state[prev_state_ind % 30] = state;
	return_state = prev_state_ind;
	prev_state_ind = prev_state_ind + 1;
	state = new_state;

	return return_state;
}

void restore_state(int16_t state_index) {
	/**************************************************************************
	 * Restores FSM from a case of sending quick messages.
	 * Input: States to avoid
	 * Output: None
	 **************************************************************************/
	state = previous_state[state_index % 30];
	state_to_restore = FSM_INVALID_STATE;
}
