# define __USE_MISC
# include <math.h>
# undef __USE_MISC
# include <stdio.h>
# include "coordonne_absolue.h"
# include "mv_statemachine.h"
# include "mv_statemachine_states.h"


/* ── État global ──────────────────────────────────────────────────────────── */

Pose robot_pose = { .x = 0.0f, .y = 0.0f, .theta = 0.0f };


/* ── Calculs purs ─────────────────────────────────────────────────────────── */

float compute_target_angle(Pose from, Point2D target)
{
    return atan2f(target.y - from.y, target.x - from.x);
}

float compute_relative_rotation(float current_theta, float target_angle, Face face)
{
    float delta = target_angle - current_theta;

    // si on va en marche arrière, on pivote vers l'angle opposé
    if (face == FACE_ARRIERE)
        delta += (float)M_PI;

    // normalise dans [-π, π]
    if (delta >  (float)M_PI) delta -= 2.0f * (float)M_PI;
    if (delta < -(float)M_PI) delta += 2.0f * (float)M_PI;

    return delta;
}

float compute_distance(Pose from, Point2D target, Face face)
{
    float dist = sqrtf(
        (target.x - from.x) * (target.x - from.x) +
        (target.y - from.y) * (target.y - from.y)
    );

    return (face == FACE_ARRIERE) ? -dist : dist;
}

void robot_add_distance(float distance)
{
    robot_pose.x = robot_pose.x + distance * sin(robot_pose.theta);
    robot_pose.y = robot_pose.y + distance * cos(robot_pose.theta);
}

void robot_add_rotation(float angle)
{
    robot_pose.theta += angle;
}


/* ── Mise à jour de pose ──────────────────────────────────────────────────── */

void robot_update_pose(float new_x, float new_y, float new_theta)
{
    robot_pose.x     = new_x;
    robot_pose.y     = new_y;
    robot_pose.theta = new_theta;
}
void robot_update_x(float x)     { robot_pose.x     = x; }
void robot_update_y(float y)     { robot_pose.y     = y; }
void robot_update_theta(float t) { robot_pose.theta = t; }


/* ── Commande principale ──────────────────────────────────────────────────── */

int goto_xy(Point2D target, float speed, Face face)
{
    if (MSM_is_busy(&MV_STATEMACHINE)) return -1;

    float target_angle = compute_target_angle(robot_pose, target);
    float rotation     = compute_relative_rotation(robot_pose.theta, target_angle, face);
    float distance     = compute_distance(robot_pose, target, face);

    MSM_reset_construction(&MV_STATEMACHINE);

    // TODO : remplacer les constantes d'accélération par des paramètres ou des defines
    MSM_enqueue_state(&MV_STATEMACHINE,
        &MV_STATE_TRANSLATION,
        genenv_mv_state_translation(0.001f, speed, rotation));

    MSM_enqueue_state(&MV_STATEMACHINE,
        &MV_STATE_TRANSLATION,
        genenv_mv_state_translation(0.001f, speed, distance));

    MSM_ready_construction(&MV_STATEMACHINE);

    return 0;
}


void robot_print_pose()
{
    printf("the current pose is x:%4.3f y%3.3f t%4.3f\n", robot_pose.x, robot_pose.y, robot_pose.theta);
}