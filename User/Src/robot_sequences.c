#include "robot_sequences.h"

#include "elevator_states.h"


// Paramètres par défaut pour les mouvements (à ajuster selon le robot)
#define SEQ_ACCEL_V   10.0f   // mm/s²
#define SEQ_SPEED_V    5.0f   // mm/s
#define SEQ_ACCEL_H   30.0f   // mm/s²
#define SEQ_SPEED_H   15.0f   // mm/s


void seq_build_homing_all(struct Sequencer* seq, float speed_V, float speed_H)
{
    sequencer_reset(seq);

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_HOME_V, genenv_elv_home_v(speed_V));

    sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_HOME_H, genenv_elv_home_h(speed_H));
}

void seq_build_grab(struct Sequencer* seq, float pos_V_mm, float pos_H_mm)
{
    sequencer_reset(seq);

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, pos_V_mm));

    sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, pos_H_mm));
    // HOLD automatique via idle enregistré dans le séquenceur
}

void seq_build_deposit(struct Sequencer* seq, float pos_V_mm, float pos_H_mm)
{
    sequencer_reset(seq);

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, pos_V_mm));

    sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, pos_H_mm));
    // HOLD automatique via idle enregistré dans le séquenceur
}

void seq_add_deplacement_H(struct Sequencer* seq, float pos_H_mm)
{
    //sequencer_reset(seq);

     sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, pos_H_mm));
}

void seq_add_deplacement_V(struct Sequencer* seq, float pos_V_mm)
{
    //sequencer_reset(seq);

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, pos_V_mm));

}
