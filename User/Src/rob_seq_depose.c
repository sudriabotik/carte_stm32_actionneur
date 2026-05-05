#include "rob_seq_depose.h"
#include "robot_action_autom.h"
#include "elevator_states.h"
#include <stdio.h>


// ============================================================================
// Sous-séquenceur statique pour trie_tobogan
// ============================================================================

/**
 * @brief Sous-séquenceur statique pour les actions de triage servo
 *
 * Ce séquenceur est utilisé comme sous-séquence de enchement_seq_depose.
 * Il permet d'exécuter les commandes servo immédiatement pendant le dépôt,
 * en mettant en pause la séquence principale.
 */
static struct Sequencer sub_seq_tobogan = {0};

typedef struct {
    uint8_t canal;
    uint16_t position;
} ServoParams;

// Action générique pour commander un servo
void servo_command_action(void* param) {
    ServoParams* p = (ServoParams*)param;
    i2c_servo(p->canal, p->position);
    //printf("[DBG] servo canal=%u pos=%u\n", p->canal, p->position);
}

static ServoParams servo_params_1_petite_droite = {1, POSITION_PETITE_DROITE};
static ServoParams servo_params_1_droite = {1, POSITION_DROITE};
static ServoParams servo_params_1_petite_gauche = {1, POSITION_PETITE_GAUCHE};
static ServoParams servo_params_1_gauche = {1, POSITION_GAUCHE};

static ServoParams servo_params_0_petite_droite = {0, POSITION_PETITE_DROITE};
static ServoParams servo_params_0_droite = {0, POSITION_DROITE};
static ServoParams servo_params_0_petite_gauche = {0, POSITION_PETITE_GAUCHE};
static ServoParams servo_params_0_gauche = {0, POSITION_GAUCHE};

const int servo_delay = 500 ;
const int servo_delay_2 = 500 ;

void trie_tobogan_cleanup(void* param) {
    (void)param;
    
    couleur_element_jeux_1 = 0;
    couleur_element_jeux_2 = 0;
    presence_element_jeux_1 = 0;
    presence_element_jeux_2 = 0;
    
    printf("[DBG] trie_tobogan CLEANUP done\n");
}

void trie_tobogan_v2(void* param) {
    // Récupérer le séquenceur parent depuis le paramètre
    struct Sequencer* parent_seq = (struct Sequencer*)param;

    printf("[DBG] trie_tobogan_v2 START - building sub-sequence\n");

    // Réinitialiser la sous-séquence
    sequencer_reset(&sub_seq_tobogan);

    // Construire la sous-séquence avec les commandes servo

    // Servo 1 (toboggan intérieur)
    if (presence_element_jeux_1) {
        add_element_jeux_in_tob_int();

        if (couleur_element_jeux_1 != couleur_equipe) {
            // Tourner à droite
            printf("[DBG] Servo 1: droite (mauvaise couleur)\n");
            sequencer_add_action(&sub_seq_tobogan, servo_command_action, &servo_params_1_petite_droite, servo_delay);
            sequencer_add_action(&sub_seq_tobogan, servo_command_action, &servo_params_1_droite, servo_delay_2);
        }
        else {
            // Changer de couleur
            printf("[DBG] Servo 1: gauche (bonne couleur)\n");
            sequencer_add_action(&sub_seq_tobogan, servo_command_action, &servo_params_1_petite_gauche, servo_delay);
            sequencer_add_action(&sub_seq_tobogan, servo_command_action, &servo_params_1_gauche, servo_delay_2);
        }
    }

    // Servo 2 (toboggan extérieur)
    if (presence_element_jeux_2) {
        add_element_jeux_in_tob_ext();

        if (couleur_element_jeux_2 != couleur_equipe) {
            printf("[DBG] Servo 0: droite (mauvaise couleur)\n");
            sequencer_add_action(&sub_seq_tobogan, servo_command_action, &servo_params_0_petite_droite, servo_delay);
            sequencer_add_action(&sub_seq_tobogan, servo_command_action, &servo_params_0_droite, servo_delay_2);
        }
        else {
            printf("[DBG] Servo 0: gauche (bonne couleur)\n");
            sequencer_add_action(&sub_seq_tobogan, servo_command_action, &servo_params_0_petite_gauche, servo_delay);
            sequencer_add_action(&sub_seq_tobogan, servo_command_action, &servo_params_0_gauche, servo_delay_2);
        }
    }

    // Réinitialiser à la fin
    sequencer_add_action(&sub_seq_tobogan, trie_tobogan_cleanup, NULL, 0);

    // Lancer la sous-séquence (pause le parent jusqu'à la fin)
    sequencer_add_sub_sequence(parent_seq, &sub_seq_tobogan);

    printf("[DBG] trie_tobogan_v2 sub-sequence launched (%d steps)\n", sub_seq_tobogan.step_count);
}



void enchement_seq_depose(struct Sequencer* seq)
{

    printf("[DBG] START ench_seq_depose");
    sequencer_reset(seq);

    sequencer_add_action(seq, ax_servo_6_open, NULL, 100);  
    sequencer_add_action(seq, ax_servo_7_open, NULL, 300); 

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, DEPOSE_V));

    sequencer_add_action(seq,ax_servo_6_close,NULL,20);
    sequencer_add_action(seq,ax_servo_7_close,NULL,20);

    sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, DEPOSE_1_H));
    
    sequencer_add_action(seq, print_state_tobotan, NULL, 10);  // Exécuté pendant la séquence, pas maintenant

    if ( (top_place[TOB_EXT]==1) || (top_place[TOB_INT] ==1 ) )
    {
         printf("[DBG] CAS 1: Tobogan plein en haut, abandon\n");
    }

    else if ( (middle_place[TOB_EXT]==1 ) || (middle_place[TOB_INT]==1) )
    { 
        //sequencer_add_action(seq, depose_1er, NULL, 2000);
        /// DEPO 1
        printf("[DBG] CAS 2: Tobogan a éléments au milieu\n");

        sequencer_add_action(seq, scan_tobogan,  (void*)1, 0);

        if( ((element_in_ventouse[3] || element_in_ventouse[1])==0 )
            &&
            ((element_in_ventouse[2] || element_in_ventouse[0]) == 1) ) 
        {
            printf("[DBG] CAS 2a: Seulement ventouses basses (v0/v2), H -> DEPOSE_2_H\n");
            sequencer_add(seq, &elevator_H_statemachine,
                &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, DEPOSE_2_H));
        
            sequencer_add_action(seq, scan_tobogan,  (void*)2, 20);  // Scanner avant de vérifier
            sequencer_add_action(seq, reset_tobogan, NULL, 100);
            sequencer_add_action(seq, turn_off_pump_4, NULL, 700);
            //sequencer_add_action(seq, trie_tobogan, NULL, 400); // old
            sequencer_add_action(seq, trie_tobogan_v2, seq, 0);
        }

        else
        {
            printf("[DBG] CAS 2b: Ventouses hautes (v1/v3), rester à DEPOSE_1_H\n");
            sequencer_add_action(seq, reset_tobogan, NULL, 100);
            sequencer_add_action(seq, turn_off_pump_3, NULL, 700);
            //sequencer_add_action(seq, trie_tobogan, NULL, 500);
            sequencer_add_action(seq, trie_tobogan_v2, seq, 0);
        }

        sequencer_add(seq, &elevator_H_statemachine,
            &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, DEPOSE_2_H));
        
        sequencer_add_action(seq, scan_tobogan, (void*)2, 0); 
    }
    else if ((middle_place[TOB_EXT]==0 ) && (middle_place[TOB_INT]==0))
    {
        printf("[DBG] CAS 3: Tobogan vide, 2 dépôts successifs\n");
            /// DEPO 1
        sequencer_add_action(seq, scan_tobogan,  (void*)1, 0);
        sequencer_add_action(seq, reset_tobogan, NULL, 100);
        sequencer_add_action(seq, turn_off_pump_3, NULL, 700);
        //sequencer_add_action(seq, trie_tobogan, NULL, 500);
        sequencer_add_action(seq, trie_tobogan_v2, seq, 0);

        //sequencer_add_action(seq, depose_2eme, NULL, 2000);
        sequencer_add(seq, &elevator_H_statemachine,
            &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, DEPOSE_2_H));

        /// DEPO 2
        sequencer_add_action(seq, scan_tobogan,  (void*)2, 20);  // Scanner avant de vérifier
        sequencer_add_action(seq, reset_tobogan, NULL, 100);
        sequencer_add_action(seq, turn_off_pump_4, NULL, 700);
        //sequencer_add_action(seq, trie_tobogan, NULL, 400);
        sequencer_add_action(seq, trie_tobogan_v2, seq, 0);

    }

}