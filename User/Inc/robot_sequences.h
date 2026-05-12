#ifndef __ROBOT_SEQUENCES_H
#define __ROBOT_SEQUENCES_H

/**
 * @file robot_sequences.h
 *
 * @brief Séquences de haut niveau pour le robot : homing, saisie, dépôt, etc.
 *
 * Chaque fonction "seq_build_*" remplit un Sequencer avec les étapes
 * correspondant à l'action. Après l'appel, démarrer avec sequencer_start().
 *
 * Les séquences utilisent les deux machines à états des ascenseurs :
 *   - elevator_V_statemachine  (moteur droit, axe vertical)
 *   - elevator_H_statemachine  (moteur gauche, axe horizontal)
 *
 * Exemple :
 *   struct Sequencer seq = sequencer_init();
 *   seq_build_homing_all(&seq);
 *   sequencer_start(&seq);
 *
 *   // Dans l'interrupt :
 *   sequencer_update(&seq);
 *   MSM_update(&elevator_V_statemachine, dt);
 *   MSM_update(&elevator_H_statemachine, dt);
 */

#include "sequencer.h"


/**
 * @brief Séquence de homing complet :
 *        1. Home vertical  (remonte jusqu'à l'endstop)
 *        2. Home horizontal (recule jusqu'à l'endstop)
 *
 * @param seq      Séquenceur à remplir (doit être initialisé ou remis à zéro).
 * @param speed_V  Vitesse de homing vertical en % PWM (ex: -20.0f).
 * @param speed_H  Vitesse de homing horizontal en % PWM (ex: -10.0f).
 */
void seq_build_homing_all(struct Sequencer* seq, float speed_V, float speed_H);


/**
 * @brief Séquence de saisie d'un objet :
 *        1. Déplace l'ascenseur vertical à la position de saisie
 *        2. Déplace l'ascenseur horizontal à la position de saisie
 *        3. Maintient la position verticale (hold)
 *
 * @param seq       Séquenceur à remplir.
 * @param pos_V_mm  Position verticale cible en mm.
 * @param pos_H_mm  Position horizontale cible en mm.
 */
void seq_build_grab(struct Sequencer* seq);

void seq_ready_to_grap(struct Sequencer* seq); // position juste avant la position tous en bas. 

void seq_build_deposit(struct Sequencer* seq);

void seq_safe_position(struct Sequencer* seq);

void seq_deplacement_H(struct Sequencer* seq, float pos_H_mm);

void seq_deplacement_V(struct Sequencer* seq, float pos_V_mm);

void seq_open_pince(struct Sequencer* seq);

void seq_close_pince(struct Sequencer* seq);

void seq_ejecter_elements(struct Sequencer* seq, int num_element_a_ejecter);

void seq_ejecter_1_element(struct Sequencer* seq);

void seq_fermer_porte_et_rentrer_ax(struct Sequencer* seq);

/**
 * @brief Séquence combinée : ferme la porte, rentre l'AX, puis éjecte 1 élément
 *
 * Cette fonction enchaîne automatiquement deux séquences :
 * 1. Fermer porte et rentrer AX (1000ms)
 * 2. Éjecter 1 élément (réévalue l'état après l'étape 1)
 *
 * @param seq Séquenceur à remplir
 */
void seq_fermer_puis_ejecter_1_element(struct Sequencer* seq);

void seq_ax_safe_pos_for_calage(struct Sequencer* seq);

void seq_ax_open_cursor(struct Sequencer* seq);

void seq_ax_fermer_cursor(struct Sequencer* seq);

void seq_grap_pince(struct Sequencer* seq);

void seq_open_cursor_2(struct Sequencer* seq);

void seq_fermeture_cursor_2(struct Sequencer* seq);

// ============================================================================
// Séquences de dépôt déplacées vers rob_seq_depose.h
// ============================================================================
#include "rob_seq_depose.h"

#endif // __ROBOT_SEQUENCES_H
