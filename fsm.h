#include <stdint.h>

typedef enum {
	START_STATE = 0,
	START_WAIT,
	PB_HOLD,
	PB_ORANGE,
	PB_RED,
	PB_CROSSING,
	TRAIN_ORANGE,
	TRAIN_RED,
	TRAIN_PASSED,
	MAINTENANCE_ORANGE,
	MAINTENANCE_RED,
	MAINTENANCE_BLUE_ON,
	MAINTENANCE_BLUE_OFF
} state_t;

//typedef enum {
//	STATE_RESET = 0,
//	STATE_1,
//	STATE_2,
//	STATE_3,
//	STATE_4
//} state_t;

/* Initialize the finite state machine */
void fsm_init(void);

/* Acquire a lock on the fsm before accessing/altering state */
#define FSM_LOCK_FAILED   (0)
#define FSM_LOCK_ACQUIRED (1)
uint8_t fsm_lock(void);
void fsm_unlock(void);

/* Query FSM state - only do this when you have acquired the lock! */
state_t fsm_get_state(void);

/* Alter FSM state - only do this when you have acquired the lock! */
void fsm_set_state(state_t new_state);
