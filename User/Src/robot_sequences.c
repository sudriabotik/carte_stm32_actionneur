#include "robot_sequences.h"
#include "robot_action_autom.h"
#include "elevator_states.h"


// Paramètres par défaut pour les mouvements (à ajuster selon le robot)
#define SEQ_ACCEL_V   30.0f   // mm/s²
#define SEQ_SPEED_V    50.0f   // mm/s
#define SEQ_ACCEL_H   50.0f   // mm/s²
#define SEQ_SPEED_H   70.0f   // mm/s

// position enregistrer : 
#define GRAP_H -497.0f 
#define READY_TO_GRAP_V -200.0f
#define GRAP_V -232.0f

#define SAFE_POSITION_H -300.0f // position des ascenseur pour le perimetrer non déployer 

#define DEPOSE_V 5.0f
#define DEPOSE_1_H -105.0f
#define DEPOSE_2_H 7.0f

void seq_build_homing_all(struct Sequencer* seq, float speed_V, float speed_H)
{
    sequencer_reset(seq);

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_HOME_V, genenv_elv_home_v(speed_V));

    sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_HOME_H, genenv_elv_home_h(speed_H));
}

void seq_open_pince(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, ax_servo_6_open, NULL, 0);  
    sequencer_add_action(seq, ax_servo_7_open, NULL, 0); 
}

void seq_close_pince(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, ax_servo_6_close, NULL, 0);  
    sequencer_add_action(seq, ax_servo_7_close, NULL, 0); 
}

void seq_safe_position(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add(seq, &elevator_H_statemachine,
            &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, SAFE_POSITION_H));
    sequencer_add(seq, &elevator_V_statemachine,
            &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, DEPOSE_V));
}

void seq_ready_to_grap(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add(seq, &elevator_H_statemachine,
            &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, GRAP_H));
    sequencer_add(seq, &elevator_V_statemachine,
            &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, READY_TO_GRAP_V));
}

void seq_build_grab(struct Sequencer* seq)
{
    sequencer_reset(seq);

    sequencer_add_action(seq, ax_servo_6_open, NULL, 0);  
    sequencer_add_action(seq, ax_servo_7_open, NULL, 0); 

    sequencer_add(seq, &elevator_H_statemachine,
                &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, GRAP_H));

    sequencer_add_action(seq, turn_on_pump_4, NULL, 0);  
    sequencer_add_action(seq, turn_on_pump_3, NULL, 0);

    sequencer_add_action(seq, ax_servo_6_grap, NULL, 0);  
    sequencer_add_action(seq, ax_servo_7_grap, NULL, 20); 
    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, GRAP_V));

    // HOLD automatique via idle enregistré dans le séquenceur
}

void seq_build_deposit(struct Sequencer* seq)
{
    sequencer_reset(seq);

    sequencer_add_action(seq, reset_tobogan, NULL, 0);
    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, DEPOSE_V));

    sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, DEPOSE_1_H));
    
    sequencer_add_action(seq, scan_tobogan,NULL,0);
    sequencer_add_action(seq, turn_off_pump_3, NULL, 500);  
    sequencer_add_action(seq, trie_tobogan, NULL, 2000);

    sequencer_add(seq, &elevator_H_statemachine,
                &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, DEPOSE_2_H));
    sequencer_add_action(seq, reset_tobogan, NULL, 0);

    sequencer_add_action(seq, scan_tobogan, NULL, 500); 
    sequencer_add_action(seq, turn_off_pump_4, NULL, 500); 
    sequencer_add_action(seq, trie_tobogan, NULL, 2000);

    sequencer_add_action(seq,servo_porte_fermer,NULL,10);

    sequencer_add_action(seq, ax_caca_rentrer, NULL, 0);
    //sequencer_add_action(seq, ax_servo_6_close, NULL, 0);   // Pas de délai
    //sequencer_add_action(seq, ax_servo_7_close, NULL, 0); 
}

void seq_ejecter_elements(struct Sequencer* seq, int num_element_a_ejecter)
{ 
    // TO DOfaut faire la logique des elements de jeux du tobogan pour bien gerer l'expulsion des elements de jeux 

    sequencer_reset(seq);

    sequencer_add_action(seq,servo_porte_ouvert,NULL,500);
    sequencer_add_action(seq,ax_caca_milieu,NULL,1000);
    sequencer_add_action(seq,ax_caca_ejecter,NULL,500);
    sequencer_add_action(seq,servo_porte_fermer,NULL,100);
    sequencer_add_action(seq,ax_caca_rentrer,NULL,500 );
}

void seq_deplacement_H(struct Sequencer* seq, float pos_H_mm)
{
    sequencer_reset(seq);

    sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, pos_H_mm));

}

void seq_deplacement_V(struct Sequencer* seq, float pos_V_mm)
{
    sequencer_reset(seq);

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, pos_V_mm));

}

