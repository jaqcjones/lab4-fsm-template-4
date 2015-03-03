/*
 * systick.c
 */

#include "systick.h"
#include "fsm.h"

static uint32_t count = 0;
static uint32_t count_to_val;
static void(*callback_fn)();

void __attribute__ ((interrupt)) systick_handler(void)
{
	if(callback_fn) {
		callback_fn();
	}
}

void systick_init(uint32_t end_val) {

	callback_fn = systick_1sec_callback_fn;
	count_to_val = end_val; /* Timer end value */
	uint32_t x = 0;
	uint32_t y = 0x7;
	SYSTICK->CS = x;			/* Step 1 - Disable timer */
	SYSTICK->RV = WAIT_CYCLES;	/* Load # cycles to wait before interrupt */
	SYSTICK->CV = x;			/* Set current value to 0 */
	SYSTICK->CS = y;			/* Load 7 to start interrupts */
}

void systick_on(void) {
	/* Set current value to 0 */
	SYSTICK->CV = 0;
	/* Start 1 second interrupts */
	SYSTICK->CS = 0x7;
}

void systick_off(void) {
	/* Disable timer */
	SYSTICK->CS = 0;
}

void systick_1sec_callback_fn(void) {
	/* Timer counts in 1 sec intervals to desired value */
	count = count + 1;
	if(count >= count_to_val && fsm_lock() == FSM_LOCK_ACQUIRED) {
		systick_off();
		count = 0;
		switch(fsm_get_state()) {
		case START_STATE:
			fsm_set_state(START_WAIT);
			break;
		case PB_HOLD:
			fsm_set_state(PB_ORANGE);
			break;
		case PB_ORANGE:
			fsm_set_state(PB_RED);
			break;
		case PB_RED:
			fsm_set_state(PB_CROSSING);
			break;
		case PB_CROSSING:
			fsm_set_state(START_STATE);
			break;
		case TRAIN_ORANGE:
			fsm_set_state(TRAIN_RED);
			break;
		case TRAIN_PASSED:
			fsm_set_state(START_STATE);
			break;
		case MAINTENANCE_ORANGE:
			fsm_set_state(MAINTENANCE_RED);
			break;
		case MAINTENANCE_RED:
			fsm_set_state(MAINTENANCE_BLUE_ON);
			break;
		case MAINTENANCE_BLUE_ON:
			fsm_set_state(MAINTENANCE_BLUE_OFF);
			break;
		case MAINTENANCE_BLUE_OFF:
			fsm_set_state(MAINTENANCE_BLUE_ON);
			break;
		default:
			fsm_set_state(START_STATE);
			break;
		}
		fsm_unlock();
	}
}



