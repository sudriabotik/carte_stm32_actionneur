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
#define GRAP_V -226.0f

#define DEPOSE_1_H -110.0f
#define DEPOSE_2_H 6.0f

void seq_build_homing_all(struct Sequencer* seq, float speed_V, float speed_H)
{
    sequencer_reset(seq);

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_HOME_V, genenv_elv_home_v(speed_V));

    sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_HOME_H, genenv_elv_home_h(speed_H));
}

void seq_build_grab(struct Sequencer* seq)
{
    sequencer_reset(seq);

    sequencer_add(seq, &elevator_H_statemachine,
                &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, GRAP_H));

    sequencer_add_action(seq, turn_on_pump_4, NULL, 0);  
    sequencer_add_action(seq, turn_on_pump_3, NULL, 0);
    sequencer_add_action(seq, ax_servo_6_open, NULL, 0);  
    sequencer_add_action(seq, ax_servo_7_open, NULL, 0); 

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, GRAP_V));
    // HOLD automatique via idle enregistré dans le séquenceur
}

void seq_build_deposit(struct Sequencer* seq)
{
    sequencer_reset(seq);

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, 0));

    sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, DEPOSE_1_H));
    // HOLD automatique via idle enregistré dans le séquenceur
    //sequencer_add_action(seq, turn_off_pump_4, NULL, 0);   // Pas de délai
    sequencer_add_action(seq, turn_off_pump_3, NULL, 500); 
    sequencer_add_action(seq, trie_tobogan, NULL, 2000);
    sequencer_add(seq, &elevator_H_statemachine,
                &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, DEPOSE_2_H));
    sequencer_add_action(seq, reset_tobogan, NULL, 0); 
    sequencer_add_action(seq, turn_off_pump_4, NULL, 500); 
    sequencer_add_action(seq, trie_tobogan, NULL, 2000);
    //sequencer_add_action(seq, ax_servo_6_close, NULL, 0);   // Pas de délai
    //sequencer_add_action(seq, ax_servo_7_close, NULL, 0); 
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

