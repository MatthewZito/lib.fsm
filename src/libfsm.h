#ifndef LIB_FSM_H
#define LIB_FSM_H

#include <stdbool.h>

#define MAX_STATE_NAME_SIZE 32
#define MAX_FSM_NAME_SIZE 32
#define MAX_INBUF_SIZE 128
#define MAX_TRANSITION_TABLE_SIZE 128
#define MAX_TRANSITION_KEY_SIZE 64
#define MAX_OUTBUF_SIZE 1024

typedef struct state state_t;
typedef struct fsm fsm_t;

/**
 * @brief Wrapper for boolean state flag
 */
typedef enum {
	FSM_FALSE,
	FSM_TRUE,
} fsm_bool_t;

/**
 * @brief FSM Error states
 */
typedef enum {
	FSM_SUCCESS,
	FSM_E_FAILED_TRANSITION,
	FSM_E_GENERAL_ERROR,
	FSM_E_INVALID_INPUTS,
} fsm_error_t;

typedef struct fsm_outbuf {
	char outbuf[MAX_OUTBUF_SIZE];
	unsigned int curr_pos;
} fsm_outbuf_t;

typedef void(*outhandler)(
	state_t*,
	state_t*,
	char*,
	unsigned int,
	fsm_outbuf_t*
);

typedef struct tt_entry {
	char transition_key[MAX_TRANSITION_KEY_SIZE];
	unsigned int transition_key_size;
	state_t* next_state;
	outhandler out_handler;
} tt_entry_t;

/**
 * @brief Table for tracking state transitions
 */
typedef struct tt {
	tt_entry_t entry[MAX_TRANSITION_TABLE_SIZE];
} tt_t;

/**
 * @brief Represents the state of a Finite State Machine
 */
typedef struct state {
	/* The name of the state instance */
	char name[MAX_STATE_NAME_SIZE];

	/* Transition Table for the state instance */
	tt_t transition_table;

	/*Flag indicating whether the current state is final */
	fsm_bool_t is_final;
} state_t;

/**
 * @brief Represents a Finite State Machine
 */
typedef struct fsm {
	/* Initial state of the FSM */
	state_t* initial_state;

	/* The name of the FSM instance */
	char name[MAX_FSM_NAME_SIZE];

	/* Application-provided input */
	char inbuf[MAX_INBUF_SIZE];

	/* Current size of input_buffer */
	unsigned int inbuf_size;

	/* N bytes of input_buffer that has been processed */
	unsigned int inbuf_cursor;
} fsm_t;

/* FSM API */

fsm_t* fsm_init(const char* name);

state_t* fsm_state_init(const char* name, fsm_bool_t is_final);

bool fsm_set_initial_state(fsm_t* fsm, state_t* state);

bool fsm_add_tt_entry(
	tt_t* t_table,
	char* transition_key,
	unsigned int sizeof_key,
	state_t* next_state,
	outhandler output_handler
);

fsm_error_t fsm_invoke(
	fsm_t* fsm, char*
	inbuf,
	unsigned int size,
	fsm_outbuf_t* outbuf,
	fsm_bool_t* fsm_result
);

#endif /* LIB_FSM_H */
