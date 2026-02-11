#include "mv_statemachine_states.h"



# include <inttypes.h>
# include <math.h>

# include "tim.h"

# include "state_machine.h"

# include "robot_data.h"
# include "motor_asserv.h"
# include "pid.h"
# include "pid_config.h"
# include "recorder.h"
# include "mathfuncs.h"
# include "logicfuncs.h"



/*
##########
STATE HOLD
##########
*/

void state_hold_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("holding the robot in place");

	// reset the encoder, as such we can use the distance to zero as the travelled distance.
	Encoder16Reset(&encoder_R);
	Encoder16Reset(&encoder_L);

	// pregenerates a trapezoidal function with the correct parameters
	env->pos_slope = pregen_position_slope(env->acceleration, movement_control.speed, movement_control.dist);

	// resets all used PID runtimes
	PID_reset_runtime(&pid_translation_runtime);
	PID_reset_runtime(&pid_rotation_runtime);
	PID_reset_runtime(&pid_motor_R_runtime);
	PID_reset_runtime(&pid_motor_L_runtime);

	movement_control.elapsed_time = 0;
}

void state_hold_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	Encoder16Update(&encoder_R);
	Encoder16Update(&encoder_L);

	movement_control.elapsed_time += delta_time;

	float distance_travelled = (encoder_R.total_count + encoder_L.total_count) / 2;
	distance_travelled = (distance_travelled / encoder_R.ticks_per_revolution) * M_PI * ROBOT_ENCODER_WHEEL_DIAMETER;
	float rotation_error = (encoder_R.total_count - encoder_L.total_count) / 2;
	rotation_error = (rotation_error / encoder_R.ticks_per_revolution) * M_PI * ROBOT_ENCODER_WHEEL_DIAMETER; // INCORRECT, TEMP

	float motor_command_position = PID_Run(&pid_translation_runtime, &pid_translation, distance_travelled, 0, delta_time);
	float motor_command_rotation = PID_Run(&pid_rotation_runtime, &pid_rotation, rotation_error, 0, delta_time);

	//printf("translation pid command : %2.3f\n", motor_command_position);
	//printf("rotation pid command : %2.3f\n", motor_command_rotation);

	

	motor_drive_pid(delta_time, motor_command_position + motor_command_rotation, motor_R, encoder_R, pid_motor_R, &pid_motor_R_runtime);
	motor_drive_pid(delta_time, motor_command_position - motor_command_rotation, motor_L, encoder_L, pid_motor_L, &pid_motor_L_runtime);
}

void state_hold_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("stopped holding the robot\n");

	motor_drive(motor_R, 0.0f);
	motor_drive(motor_L, 0.0f);
}

struct State MOVEMENT_STATE_HOLD =
{
	.wake = state_hold_wake,
	.run = state_hold_run,
	.stop = state_hold_stop
};
