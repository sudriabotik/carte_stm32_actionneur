#include "elevator_states.h"

#include <math.h>
#include <stdio.h>

#include "robot_data.h"
#include "motor_asserv.h"
#include "pid.h"
#include "pid_config.h"
#include "mathfuncs.h"
#include "logicfuncs.h"
#include "encoders.h"
#include "motors.h"

#include "gpio.h"
#include "main.h"

float abs_pos_V_mm = 0;
float abs_pos_H_mm = 0;

// Endstop vertical : PULLDOWN, repos = RESET, cliqué = SET
#define ENDSTOP_V_TRIGGERED() (HAL_GPIO_ReadPin(end_switch_vertical_GPIO_Port, end_switch_vertical_Pin) == GPIO_PIN_SET)

// Endstop horizontal : PULLDOWN, repos = RESET, cliqué = SET
#define ENDSTOP_H_TRIGGERED() (HAL_GPIO_ReadPin(end_switch_horizontal_GPIO_Port, end_switch_horizontal_Pin) == GPIO_PIN_SET)


// Machines à états des 2 ascenseurs — initialisées dans main via MSM_init()
struct MvStateMachine elevator_V_statemachine;
struct MvStateMachine elevator_H_statemachine;


static float ticks_to_mm_V(int32_t ticks)
{
	return ((float)ticks / encoder_R.ticks_per_revolution) * M_PI * ROBOT_ENCODER_WHEEL_DIAMETER;
}

static float ticks_to_mm_H(int32_t ticks)
{
	return ((float)ticks / encoder_L.ticks_per_revolution) * M_PI * ROBOT_ENCODER_WHEEL_DIAMETER;
}

//////// 
// calcul abs_position ascenseur 
/////


/*
############################
ELV_STATE_MOVE_V  (vertical)
############################
*/

void elv_move_v_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	// env->distance contient la position absolue cible
	// Calculer la distance relative à parcourir
	float distance_relative = env->distance - abs_pos_V_mm;

	printf("elevator V : moving from %.1f mm to %.1f mm (distance: %.1f mm)\n",
	       abs_pos_V_mm, env->distance, distance_relative);

	Encoder16Reset(&encoder_R);
	// Utiliser la distance RELATIVE pour la trajectoire
	env->pos_slope   = pregen_position_slope(env->acceleration, env->speed, distance_relative);
	env->elapsed_time = 0;

	PID_reset_runtime(&pid_position_elevator_V_runtime);
}

void elv_move_v_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	Encoder16Update(&encoder_R);
	env->elapsed_time += delta_time;

	float current_pos    = ticks_to_mm_V(encoder_R.total_count);
	float desired_pos    = eval_position_slope(env->elapsed_time, env->pos_slope);

	env->real_outcome = current_pos; 

	float motor_command  = PID_Run(&pid_position_elevator_V_runtime, &pid_position_elevator_V,
	                               current_pos, desired_pos, delta_time);

	motor_drive(motor_R, motor_command);

	// Calculer la distance relative à parcourir pour la comparaison
	float distance_relative = env->distance - abs_pos_V_mm;

	if (is_val_near(current_pos, distance_relative, 3.0f) && fabs(pid_position_elevator_V_runtime.d) < 0.05f)
	{
		env->real_outcome = current_pos;
		MSM_set_state_finished(statemachine);
	}
}

void elv_move_v_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	// Mettre à jour la position absolue : ancienne position + distance parcourue
	abs_pos_V_mm += env->real_outcome;

	printf("elevator V : reached %.1f mm (target was %.1f mm)\n", abs_pos_V_mm, env->distance);
	motor_drive(motor_R, 0.0f);
}

struct MvState ELV_STATE_MOVE_V =
{
	.wake = elv_move_v_wake,
	.run  = elv_move_v_run,
	.stop = elv_move_v_stop
};

struct MvStateEnv genenv_elv_move_v(float acceleration, float speed, float target_abs_pos_v)
{

	struct MvStateEnv tmp = { .acceleration = acceleration, .speed = speed, .distance = target_abs_pos_v };
	return tmp;
}



/*
############################
ELV_STATE_HOLD_V  (vertical)
############################
*/

void elv_hold_v_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("elevator V : holding position wake \r\n");

	Encoder16Reset(&encoder_R);
	PID_reset_runtime(&pid_position_elevator_V_runtime);
	PID_reset_runtime(&pid_motor_R_runtime);
}

void elv_hold_v_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	Encoder16Update(&encoder_R);

	float current_pos   = ticks_to_mm_V(encoder_R.total_count);
	// PID position → sortie directement en % PWM (pas de boucle de vitesse)
	float motor_command = PID_Run(&pid_position_elevator_V_runtime, &pid_position_elevator_V,
	                              current_pos, 0.0f, delta_time);

	//printf("pos=%.1f cmd=%.1f\n", current_pos, motor_command);

	motor_drive(motor_R, motor_command);
}

void elv_hold_v_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("elevator V : released hold\n");
	motor_drive(motor_R, 0.0f);
}

struct MvState ELV_STATE_HOLD_V =
{
	.wake = elv_hold_v_wake,
	.run  = elv_hold_v_run,
	.stop = elv_hold_v_stop
};

struct MvStateEnv genenv_elv_hold_v()
{
	struct MvStateEnv tmp = {};
	return tmp;
}



/*
############################
ELV_STATE_HOME_V  (vertical)
############################
*/

void elv_home_v_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("elevator V : homing wake\n");
	// TODO : vérifier le signe de env->speed selon le sens mécanique (positif ou négatif)
}

void elv_home_v_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	// Avance à vitesse fixe vers l'endstop (env->speed en %, entre -100 et +100)
	motor_drive(motor_R, env->speed);
	
	if (ENDSTOP_V_TRIGGERED())
	{
		printf("endstop_vertical\n");
		MSM_set_state_finished(statemachine);
	}
}

void elv_home_v_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("elevator V : endstop reached, resetting position to 0\n");
	motor_drive(motor_R, 0.0f);
	// Remet le zéro de position
	Encoder16Reset(&encoder_R);
	// Réinitialiser la position absolue
	abs_pos_V_mm = 0.0f;
}

struct MvState ELV_STATE_HOME_V =
{
	.wake = elv_home_v_wake,
	.run  = elv_home_v_run,
	.stop = elv_home_v_stop
};

// speed : valeur en % PWM direct (-100 à +100), signe détermine le sens vers l'endstop
struct MvStateEnv genenv_elv_home_v(float speed)
{
	struct MvStateEnv tmp = { .speed = speed };
	return tmp;
}



/*
##############################
ELV_STATE_MOVE_H  (horizontal)
##############################
*/

void elv_move_h_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	// env->distance contient la position absolue cible
	// Calculer la distance relative à parcourir
	float distance_relative = env->distance - abs_pos_H_mm;

	printf("elevator H : moving from %.1f mm to %.1f mm (distance: %.1f mm)\n",
	       abs_pos_H_mm, env->distance, distance_relative);

	Encoder16Reset(&encoder_L);
	// Utiliser la distance RELATIVE pour la trajectoire
	env->pos_slope    = pregen_position_slope(env->acceleration, env->speed, distance_relative);
	env->elapsed_time = 0;

	PID_reset_runtime(&pid_position_elevator_H_runtime);
}

void elv_move_h_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	Encoder16Update(&encoder_L);
	env->elapsed_time += delta_time;

	float current_pos   = ticks_to_mm_H(encoder_L.total_count);
	float desired_pos   = eval_position_slope(env->elapsed_time, env->pos_slope);

	env->real_outcome = current_pos;

	float motor_command = PID_Run(&pid_position_elevator_H_runtime, &pid_position_elevator_H,
	                              current_pos, desired_pos, delta_time);

	motor_drive(motor_L, motor_command);

	// Calculer la distance relative à parcourir pour la comparaison
	float distance_relative = env->distance - abs_pos_H_mm;

	if (is_val_near(current_pos, distance_relative, 5.0f))
	{
		env->real_outcome = current_pos;
		MSM_set_state_finished(statemachine);
	}
}

void elv_move_h_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	// Mettre à jour la position absolue : ancienne position + distance parcourue
	abs_pos_H_mm += env->real_outcome;

	printf("elevator H : reached %.1f mm (target was %.1f mm)\n", abs_pos_H_mm, env->distance);
	motor_drive(motor_L, 0.0f);
}

struct MvState ELV_STATE_MOVE_H =
{
	.wake = elv_move_h_wake,
	.run  = elv_move_h_run,
	.stop = elv_move_h_stop
};

struct MvStateEnv genenv_elv_move_h(float acceleration, float speed, float distance)
{
	struct MvStateEnv tmp = { .acceleration = acceleration, .speed = speed, .distance = distance };
	return tmp;
}


/*
##############################
ELV_STATE_HOLD_H  (horizontal)
##############################
*/

void elv_hold_h_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("elevator H : holding position\n");

	Encoder16Reset(&encoder_L);
	PID_reset_runtime(&pid_position_elevator_H_runtime);
}

void elv_hold_h_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	Encoder16Update(&encoder_L);

	float current_pos   = ticks_to_mm_H(encoder_L.total_count);
	float motor_command = PID_Run(&pid_position_elevator_H_runtime, &pid_position_elevator_H,
	                              current_pos, 0.0f, delta_time);

	motor_drive(motor_L, motor_command);
}

void elv_hold_h_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("elevator H : released hold\n");
	motor_drive(motor_L, 0.0f);
}

struct MvState ELV_STATE_HOLD_H =
{
	.wake = elv_hold_h_wake,
	.run  = elv_hold_h_run,
	.stop = elv_hold_h_stop
};

struct MvStateEnv genenv_elv_hold_h()
{
	struct MvStateEnv tmp = {};
	return tmp;
}



/*
##############################
ELV_STATE_HOME_H  (horizontal)
##############################
*/

void elv_home_h_wake(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("elevator H : homing wake\n");
	// TODO : vérifier le signe de env->speed selon le sens mécanique (positif ou négatif)
}

void elv_home_h_run(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	// Avance à vitesse fixe vers l'endstop (env->speed en %, entre -100 et +100)
	motor_drive(motor_L, env->speed);

	if (ENDSTOP_H_TRIGGERED())
	{
		MSM_set_state_finished(statemachine);
	}
}

void elv_home_h_stop(struct MvStateMachine* statemachine, struct MvStateEnv* env, float delta_time)
{
	printf("elevator H : endstop reached, resetting position to 0\n");
	motor_drive(motor_L, 0.0f);
	// Remet le zéro de position
	Encoder16Reset(&encoder_L);
	// Réinitialiser la position absolue
	abs_pos_H_mm = 0.0f;
}

struct MvState ELV_STATE_HOME_H =
{
	.wake = elv_home_h_wake,
	.run  = elv_home_h_run,
	.stop = elv_home_h_stop
};

// speed : valeur en % PWM direct (-100 à +100), signe détermine le sens vers l'endstop
struct MvStateEnv genenv_elv_home_h(float speed)
{
	struct MvStateEnv tmp = { .speed = speed };
	return tmp;
}
