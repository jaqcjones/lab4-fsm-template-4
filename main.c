#include "fsm.h"
#include "USART2.h"
#include "userbutton.h"
#include "servo.h"
#include "systick.h"


void USART2_callback_fn(uint8_t byte) {
	if (fsm_lock() == FSM_LOCK_ACQUIRED) {
		if(byte == 'c') {
			switch(fsm_get_state()) {
			case TRAIN_RED:
				fsm_set_state(TRAIN_PASSED);
				break;
			case MAINTENANCE_BLUE_ON:
			case MAINTENANCE_BLUE_OFF:
				fsm_set_state(START_STATE);
				break;
			default:
				fsm_set_state(fsm_get_state());
				break;
			}
		}
		else if(byte == 'h') {
			fsm_set_state(MAINTENANCE_ORANGE);
		}
		else if(byte == 'a') {
			fsm_set_state(TRAIN_ORANGE);
		}
		else {
			switch(fsm_get_state()) {
			case PB_HOLD:
				if(byte=='t')
					fsm_set_state(PB_ORANGE);
				break;
			case PB_ORANGE:
				fsm_set_state(PB_ORANGE);
				break;
			default:
				fsm_set_state(fsm_get_state());
				break;
			}
		}
		fsm_unlock();
	}
}


void userbutton_callback_fn(void) {
	if(fsm_lock() == FSM_LOCK_ACQUIRED) {
		switch(fsm_get_state()) {
		case START_STATE:
			fsm_set_state(PB_HOLD);
			break;
		case START_WAIT:
			fsm_set_state(PB_ORANGE);
			break;
		default:
			fsm_set_state(fsm_get_state());
			break;
		}
		fsm_unlock();
	}
}

void main(void)
{
	/* Set up the USART2 9600-8N1 and to call USART2_callback_fn when new data arrives */
	USART2_init(USART2_callback_fn);

	/* Configure user pushbutton and call pushbutton_callback_fn when button press-released */
 	userbutton_init(userbutton_callback_fn);

 	/* Initialize the servo */
 	TIM4_CH1_init();

	/* initialize the finite state machine */
	fsm_init();

	/* Enable interrupts - do this after initializing the system */
	__asm ("  cpsie i \n" );

	/* Wait here forever - everything is now interrupt driven */
	while(1)
	{
		;;;
	}
}

