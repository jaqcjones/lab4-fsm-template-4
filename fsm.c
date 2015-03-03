#include <fsm.h>

#include <LED.h>
#include <mutex.h>
#include <USART2.h>
#include <servo.h>
#include <systick.h>

static uint32_t fsm_mutex;

static state_t state = START_STATE;

uint32_t count1sec;
uint32_t count20sec;
uint32_t count3min;
uint32_t GATE_OPEN;
uint32_t GATE_CLOSED;

void fsm_init(void)
{
	count1sec = 1;
	count20sec = 2;
	count3min = 5;
	GATE_OPEN = 2100;
	GATE_CLOSED = 1100;

	mutex_init(&fsm_mutex);
	while( fsm_lock() != FSM_LOCK_ACQUIRED );
	fsm_set_state(START_STATE);
	fsm_unlock();
}

uint8_t fsm_lock(void)
{
	uint8_t retval = FSM_LOCK_FAILED;

	if( mutex_lock(&fsm_mutex) == MUTEX_LOCKED )
		retval = FSM_LOCK_ACQUIRED;

	return retval;
}

void fsm_unlock(void)
{
	mutex_unlock(&fsm_mutex);
}

state_t fsm_get_state(void)
{
	return state;
}

void fsm_set_state(state_t new_state) {
	if(fsm_mutex == MUTEX_LOCKED) {
		state = new_state;

		switch(state) {

		case START_STATE:
		default:
			/* Initialize LEDs */
			LED_init();

			/* Green LED on, ORB off */
			LED_update(LED_BLUE_OFF | LED_RED_OFF | LED_ORANGE_OFF | LED_GREEN_ON);

			/* Gate up */
			servo_test(GATE_OPEN);

			/* Start 3 min timer */
			systick_init(count3min);
			USART2_putstr("STATE: START_STATE\r\n");
			break;
		case START_WAIT:
			/* Green on, ORB off */
			LED_update(LED_BLUE_OFF | LED_RED_OFF | LED_ORANGE_OFF | LED_GREEN_ON);
			/* Gate up */
			servo_test(GATE_OPEN);
			USART2_putstr("STATE: START_WAIT\r\n");
			break;
		case PB_HOLD:
			/* Green on, ORB off */
			LED_update(LED_BLUE_OFF | LED_RED_OFF | LED_ORANGE_OFF | LED_GREEN_ON);
			/* Gate up */
			servo_test(GATE_OPEN);
			USART2_putstr("STATE: PB_HOLD\r\n");
			break;
		case PB_ORANGE:
			/* Orange on, GRB off */
			LED_update(LED_BLUE_OFF | LED_RED_OFF | LED_ORANGE_ON | LED_GREEN_OFF);
			/* Gate up */
			servo_test(GATE_OPEN);
			/* Start systick timer 1 sec */
			systick_init(count1sec);
			USART2_putstr("STATE: PB_ORANGE\r\n");
			break;
		case PB_RED:
			/* Red on, GOB off */
			LED_update(LED_BLUE_OFF | LED_RED_ON | LED_ORANGE_OFF | LED_GREEN_OFF);
			/* Gate up */
			servo_test(GATE_OPEN);
			/* Start systick timer 1 sec */
			systick_init(count1sec);
			USART2_putstr("STATE: PB_RED\r\n");
			break;
		case PB_CROSSING:
			/* Red on, GOB off */
			LED_update(LED_BLUE_OFF | LED_RED_ON | LED_ORANGE_OFF | LED_GREEN_OFF);
			/* Gate up */
			servo_test(GATE_OPEN);
			/* Start systick timer 20 sec */
			systick_init(count20sec);
			USART2_putstr("STATE: PB_CROSSING\r\n");
			break;
		case TRAIN_ORANGE:
			/* Orange on, GRB off */
			LED_update(LED_BLUE_OFF | LED_RED_OFF | LED_ORANGE_ON | LED_GREEN_OFF);
			/* Gate up */
			servo_test(GATE_OPEN);
			/* Start systick timer 1 sec */
			systick_init(count1sec);
			USART2_putstr("STATE: TRAIN_ORANGE\r\n");
			break;
		case TRAIN_RED:
			/* Red on, GOB off */
			LED_update(LED_BLUE_OFF | LED_RED_ON | LED_ORANGE_OFF | LED_GREEN_OFF);
			/* Gate down */
			servo_test(GATE_CLOSED);
			USART2_putstr("STATE: TRAIN_RED\r\n");
			USART2_putstr("MESSAGE: closed\r\n");
			break;
		case TRAIN_PASSED:
			/* Red on, GOB off */
			LED_update(LED_BLUE_OFF | LED_RED_ON | LED_ORANGE_OFF | LED_GREEN_OFF);
			/* Gate up */
			servo_test(GATE_OPEN);
			/* Start timer 20 sec */
			systick_init(count20sec);
			USART2_putstr("STATE: TRAIN_PASSED\r\n");
			break;
		case MAINTENANCE_ORANGE:
			/* Orange on, GRB off */
			LED_update(LED_BLUE_OFF | LED_RED_OFF | LED_ORANGE_ON | LED_GREEN_OFF);
			/* Gate up */
			servo_test(GATE_OPEN);
			/* Start systick timer 1 sec */
			systick_init(count1sec);
			USART2_putstr("STATE: MAINTENANCE_ORANGE\r\n");
			break;
		case MAINTENANCE_RED:
			/* Red on, GOB off */
			LED_update(LED_BLUE_OFF | LED_RED_ON | LED_ORANGE_OFF | LED_GREEN_OFF);
			/* Gate down */
			servo_test(GATE_CLOSED);
			/* Start systick timer 1 sec */
			systick_init(count1sec);
			USART2_putstr("STATE: MAINTENANCE_RED\r\n");
			USART2_putstr("MESSAGE: closed\r\n");
			break;
		case MAINTENANCE_BLUE_ON:
			/* Blue on, Red on, GO off */
			LED_update(LED_BLUE_ON | LED_RED_ON | LED_ORANGE_OFF | LED_GREEN_OFF);
			/* Gate down */
			servo_test(GATE_CLOSED);
			/* Start systick timer 1 sec */
			systick_init(count1sec);
			USART2_putstr("STATE: MAINTENANCE_BLUE_ON\r\n");
			break;
		case MAINTENANCE_BLUE_OFF:
			/* Blue off, Red on, GO off */
			LED_update(LED_BLUE_OFF | LED_RED_ON | LED_ORANGE_OFF | LED_GREEN_OFF);
			/* Gate down */
			servo_test(GATE_CLOSED);
			/* Start systick timer 1 sec */
			systick_init(count1sec);
			USART2_putstr("STATE: MAINTENANCE_BLUE_OFF\r\n");
			break;
		}
	}
}
