/*
* robot_action_autom.h
*
* fichier specifique a la mecanique du robot de chaque annee
*
* Toutes les fonctions ont la signature void fonction(void* param)
* pour être compatibles avec sequencer_add_action().
*/

#ifndef ROBOT_ACTION_AUTOM_H
#define ROBOT_ACTION_AUTOM_H

/**
 * @brief Actions pour la pince AX (servos AX-12A ID 6 et 7)
 * @param param Non utilisé (peut être NULL)
 */
void ax_pince_grap(void* param);
void ax_pince_close(void* param);
void ax_pince_open(void* param);

/**
 * @brief Actions individuelles pour chaque servo AX (évite collisions bus)
 * @param param Non utilisé (peut être NULL)
 *
 * Utilisez ces fonctions avec un délai entre elles dans le sequencer :
 *   sequencer_add_action(seq, ax_servo_6_close, NULL, 5);  // + délai 5ms
 *   sequencer_add_action(seq, ax_servo_7_close, NULL, 0);
 */
void ax_servo_6_grap(void* param);
void ax_servo_6_close(void* param);
void ax_servo_6_open(void* param);
void ax_servo_7_grap(void* param);
void ax_servo_7_close(void* param);
void ax_servo_7_open(void* param);

/**
 * @brief Actions pour les pompes à vide
 * @param param Non utilisé (peut être NULL)
 */
void turn_on_pump_4(void* param);
void turn_off_pump_4(void* param);
void turn_on_pump_3(void* param);
void turn_off_pump_3(void* param);

void trie_tobogan(void* param);
void reset_tobogan(void* param);

#endif // ROBOT_ACTION_AUTOM_H