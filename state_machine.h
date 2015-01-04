/*
 * File:   state_machine.h
 * Author: alrekr
 *
 * Created on 31 December 2014, 05:22
 */

#ifndef STATE_MACHINE_H
#define	STATE_MACHINE_H

#ifdef	__cplusplus
extern "C" {
#endif
extern int16_t state;
extern int16_t previous_state[PREVIOUS_STATE_LENGTH];
extern int8_t prev_state_ind;
extern int16_t state_to_restore;
extern bool state_changed;

void init_fsm_variables(void);
void restore_state(int16_t);
int16_t update_state(int16_t new_state);


#ifdef	__cplusplus
}
#endif

#endif	/* STATE_MACHINE_H */

