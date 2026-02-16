#ifndef __ELEVATOR_STATES_H
#define __ELEVATOR_STATES_H

/**
 * @file elevator_states.h
 *
 * @brief États de la machine à états pour les ascenseurs vertical et horizontal.
 *
 * Utilise motor_R / encoder_R pour l'ascenseur vertical.
 * Utilise motor_L / encoder_L pour l'ascenseur horizontal.
 *
 * Chaque état prend ses paramètres via MvStateEnv :
 *   - distance   : position cible en mm (depuis le zéro de l'endstop)
 *   - speed      : vitesse max en mm/s
 *   - acceleration : accélération en mm/s²
 */

#include "mv_statemachine.h"
#include "mv_statemachine_states.h"


// --- Machine à états pour chaque ascenseur ---
// Déclarées ici, définies dans elevator_states.c

extern struct MvStateMachine elevator_V_statemachine;
extern struct MvStateMachine elevator_H_statemachine;


// --- ELV_STATE_MOVE_V : déplacer l'ascenseur vertical vers une position en mm ---

extern struct MvState ELV_STATE_MOVE_V;

/**
 * @brief Génère un environnement pour ELV_STATE_MOVE_V.
 * @param acceleration  Accélération en mm/s²
 * @param speed         Vitesse max en mm/s
 * @param distance      Position cible en mm (depuis le zéro endstop)
 */
struct MvStateEnv genenv_elv_move_v(float acceleration, float speed, float distance);


// --- ELV_STATE_HOLD_V : maintenir la position de l'ascenseur vertical ---

extern struct MvState ELV_STATE_HOLD_V;

/** @brief Génère un environnement pour ELV_STATE_HOLD_V (aucun paramètre requis). */
struct MvStateEnv genenv_elv_hold_v();


// --- ELV_STATE_HOME_V : recalibration de l'ascenseur vertical sur son endstop ---

extern struct MvState ELV_STATE_HOME_V;

/**
 * @brief Génère un environnement pour ELV_STATE_HOME_V.
 * @param speed  Vitesse en % PWM direct (-100 à +100). Le signe détermine le sens vers l'endstop.
 */
struct MvStateEnv genenv_elv_home_v(float speed);


// --- ELV_STATE_MOVE_H : déplacer l'ascenseur horizontal vers une position en mm ---

extern struct MvState ELV_STATE_MOVE_H;

/**
 * @brief Génère un environnement pour ELV_STATE_MOVE_H.
 * @param acceleration  Accélération en mm/s²
 * @param speed         Vitesse max en mm/s
 * @param distance      Position cible en mm (depuis le zéro endstop)
 */
struct MvStateEnv genenv_elv_move_h(float acceleration, float speed, float distance);


// --- ELV_STATE_HOLD_H : maintenir la position de l'ascenseur horizontal ---

extern struct MvState ELV_STATE_HOLD_H;

/** @brief Génère un environnement pour ELV_STATE_HOLD_H (aucun paramètre requis). */
struct MvStateEnv genenv_elv_hold_h();


// --- ELV_STATE_HOME_H : recalibration de l'ascenseur horizontal sur son endstop ---

extern struct MvState ELV_STATE_HOME_H;

/**
 * @brief Génère un environnement pour ELV_STATE_HOME_H.
 * @param speed  Vitesse en % PWM direct (-100 à +100). Le signe détermine le sens vers l'endstop.
 */
struct MvStateEnv genenv_elv_home_h(float speed);


#endif // __ELEVATOR_STATES_H
