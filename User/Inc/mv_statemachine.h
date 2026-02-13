#ifndef __MV_STATEMACHINE_H
#define __MV_STATEMACHINE_H

/**
	* @file mv_statemachine.h
	*
	* @brief The statemachine used for movement control
	*/

# include "mathfuncs.h"

/**
	* @brief The queue size for the statemachine
	*/
# define MV_STATEMACHINE_LENGTH 4


// early declarations
struct MvStateMachine;
struct MvStateEnv;



typedef unsigned int uint_t;
typedef int int_t;

typedef void (*MvStateCall)(struct MvStateMachine*, struct MvStateEnv*, float delta_time);


enum Facing
{
	POSITIVE_X,
	POSITIVE_Y,
	NEGATIVE_X,
	NEGATIVE_Y
};

/**
 * @brief A collection of variables, used by states as arguments and persistent storage.
 * A state may not use all variables, this single struct has enough variables to accomodate all of the states.
 */
struct MvStateEnv
{
	float distance;
	float speed;
	float acceleration;

	struct FuncPositionSlope pos_slope;
	struct Trapezoid trapezoid;

	enum Facing facing;

	float elapsed_time;
	float real_outcome;
};



struct MvState
{
	MvStateCall wake;
	MvStateCall run;
	MvStateCall stop;	
};



struct MvStateMachine
{
	struct MvState* state_queue[MV_STATEMACHINE_LENGTH+1];
	struct MvStateEnv envs_queue[MV_STATEMACHINE_LENGTH+1];
	uint_t index;
	int_t state_finished;

	struct MvState* construction_state_queue[MV_STATEMACHINE_LENGTH];
	struct MvStateEnv construction_envs_queue[MV_STATEMACHINE_LENGTH];
	uint_t construction_index;
	uint_t load_construction_queue;
};



/**
 * @brief Always use this function to initialize a new statemachine struct.
 * It will initialize some fields of the struct.
 * @return The initialized struct.
 */
struct MvStateMachine MSM_init();

/**
 * @brief Empties the construction states queue, allowing it to be redefined from the start.
 * @return 0
 */
int_t MSM_reset_construction(struct MvStateMachine *machine);

/**
 * @brief Put this state and the associated environment to the construction queue.
 * @return 0 if successful, -1 id the queue is full.
 */
int_t MSM_enqueue_state(struct MvStateMachine *machine, struct MvState *state, struct MvStateEnv env);

/**
 * @brief Flags the construction queue as ready to be loaded. It will be moved to the active queue the next time the statemachine is updated.
 * @return 0
 */
int_t MSM_ready_construction(struct MvStateMachine *machine);

/**
 * @brief Get the index where the next state we add to the construction queue will be put.
 * @return The current construction index.
 */
uint_t MSM_get_construction_index(struct MvStateMachine *machine);

/**
 * @brief Check if the construction queue is full.
 * @return 1 if the construction queue is full, 0 otherwise.
 */
int_t MSM_is_construction_full(struct MvStateMachine *machine);

/**
 * @brief Check if the statemachine is still executing any state from the queue.
 * @return 1 if the statemachine is busy, 0 otherwise.
 */
int_t MSM_is_busy(struct MvStateMachine *machine);

/**
 * @brief Put this state and the associated environment to the construction queue.
 * @return 0
 */
int_t MSM_update(struct MvStateMachine *machine, float delta_time);

/**
 * @brief Indicates the current state is done, and the statemachine may go to the next one.
 * @return 0
 */
int_t MSM_set_state_finished(struct MvStateMachine *machine);



#endif
