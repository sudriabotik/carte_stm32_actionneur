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

# include <stdint.h>
#include <stdbool.h>
#include "ax_controller.h"
#include "gpio.h"
#include "i2c_servo_moteur.h"


//// POSITION AX PINCE
#define SERAGE_ax_7	 497
#define SERAGE_ax_6	 521

#define CLOSE_ax_7  190
#define CLOSE_ax_6  820

#define OUVERT_ax_7  550 // le milieu c'est 510
#define OUVERT_ax_6  470

// POSITION AX CACA 
#define POS_EJECTER_AX_CACA 200
#define POS_MILIEU_AX_CACA 400
#define POS_RENTRER_AX_CACA 810

// POSITION AX BANDEROLE
#define POS_OUVERTURE_CURSOR 820
#define POS_FERMETURE_CURSOR 495

///// POSITION SERVO_MOTEUR  
// on considère que l'on regarde le sens de rotation de face par rapport au cerveau moteur
#define POSITION_DROITE 2400
#define POSITION_MILIEU 1500
#define POSITION_GAUCHE 500

#define PORTE_FERMER SERVO_POS_0_DEG
#define PORTE_OUVERTE SERVO_POS_180_DEG

#define TOB_INT 0
#define TOB_EXT 1

// couleu de l'équipe utile pour les automs
extern uint8_t couleur_equipe;
extern uint16_t ax_caca_situation ;

extern uint8_t top_place [2] ;
extern uint8_t middle_place [2] ;
extern uint8_t bottom_place [2] ;

extern bool element_in_ventouse [4]; 


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

void ax_caca_ejecter (void* param);
void ax_caca_milieu (void* param);
void ax_caca_rentrer (void* param);

void servo_porte_fermer(void* param);
void servo_porte_ouvert(void* param);

void ax_ouverture_pour_cursor (void* param);
void ax_fermeture_pour_cursor (void* param);

/**
 * @brief Actions pour les pompes à vide
 * @param param Non utilisé (peut être NULL)
 */
void turn_on_pump_4(void* param);
void turn_off_pump_4(void* param);
void turn_on_pump_3(void* param);
void turn_off_pump_3(void* param);

void scan_tobogan(void* param);
void trie_tobogan(void* param);
void reset_tobogan(void* param);

int check_in_ventouse(void);
int check_element_jeux_tob_int(void);
int check_element_jeux_tob_ext(void);
void add_element_jeux_in_tob_int(void);
void add_element_jeux_in_tob_ext(void);
void elements_jeux_tombe(void);

void ax_caca_ejecter(void* param);
void fermer_porte_et_rentrer_ax(void* param);

void print_state_tobotan();
#endif // ROBOT_ACTION_AUTOM_H
