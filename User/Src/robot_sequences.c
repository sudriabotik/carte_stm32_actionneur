#include "robot_sequences.h"
#include "robot_action_autom.h"
#include "elevator_states.h"
#include <stdio.h>


/////////
/// DEBUT DES DEPLACEMENT DES ASCENSEURS UNIQUEMENT
//////////
void seq_safe_position(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add(seq, &elevator_H_statemachine,
            &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, SAFE_POSITION_H));
    sequencer_add(seq, &elevator_V_statemachine,
            &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, DEPOSE_V));
}

void seq_build_homing_all(struct Sequencer* seq, float speed_V, float speed_H)
{
    sequencer_reset(seq);

    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_HOME_V, genenv_elv_home_v(speed_V));

    sequencer_add(seq, &elevator_H_statemachine,
                  &ELV_STATE_HOME_H, genenv_elv_home_h(speed_H));
}

void seq_ready_to_grap(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add(seq, &elevator_H_statemachine,
            &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, GRAP_H));
    sequencer_add(seq, &elevator_V_statemachine,
            &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, READY_TO_GRAP_V));
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

/////////
/// FIN DES DEPLACEMENT DES ASCENSEURS UNIQUEMENT
//////////


/////////
/// DEBUT DES MOUVEMENTS D'AX et SERVO
//////////

void seq_open_pince(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, ax_servo_6_open, NULL, 0);  
    sequencer_add_action(seq, ax_servo_7_open, NULL, 20); 
}

void seq_close_pince(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, ax_servo_6_close, NULL, 0);  
    sequencer_add_action(seq, ax_servo_7_close, NULL, 20); 
}

void seq_grap_pince(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, ax_servo_6_grap, NULL, 0);  
    sequencer_add_action(seq, ax_servo_7_grap, NULL, 20); 
}


void seq_ax_safe_pos_for_calage(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, ax_caca_calage, NULL, 200);
}


void seq_ax_open_cursor(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, ax_ouverture_cursor, NULL, 20);
}

void seq_ax_fermer_cursor(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, ax_fermeture_cursor, NULL, 20);
}

void seq_open_cursor_2(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, ouverture_cursor_2, NULL, 20);
}

void seq_fermeture_cursor_2(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, fermeture_cursor_2, NULL, 20);
}

/////////
/// FIN DES MOUVEMENTS D'AX UNIQUEMENT
//////////

/////////
/// DEBUT MOUVEMENT COMPLEXE
//////////

void seq_build_grab(struct Sequencer* seq)
{
    sequencer_reset(seq);

    sequencer_add_action(seq, ax_servo_6_open, NULL, 0);  
    sequencer_add_action(seq, ax_servo_7_open, NULL, 0); 

    if (check_in_ventouse())
    {
        // erreur
        printf("[DBG] ventouse occuper: v0=%u v1=%u v2=%u v3=%u\n",
               element_in_ventouse[0], element_in_ventouse[1],
               element_in_ventouse[2], element_in_ventouse[3]);
        return ;
    }
    sequencer_add(seq, &elevator_H_statemachine,
                &ELV_STATE_MOVE_H, genenv_elv_move_h(SEQ_ACCEL_H, SEQ_SPEED_H, GRAP_H));

    sequencer_add_action(seq, turn_on_pump_4, NULL, 0);  
    sequencer_add_action(seq, turn_on_pump_3, NULL, 0);

    sequencer_add_action(seq, ax_servo_6_grap, NULL, 0);  
    sequencer_add_action(seq, ax_servo_7_grap, NULL, 20); 
    sequencer_add(seq, &elevator_V_statemachine,
                  &ELV_STATE_MOVE_V, genenv_elv_move_v(SEQ_ACCEL_V, SEQ_SPEED_V, GRAP_V));

    sequencer_add_action(seq, ax_servo_7_grap, NULL, 300); // juste pour mettre un delay
}

void seq_fermer_porte_et_rentrer_ax(struct Sequencer* seq)
{
    sequencer_reset(seq);
    sequencer_add_action(seq, fermer_porte_et_rentrer_ax, NULL, 1000);
}

void add_seq_fermer_porte_et_rentrer_ax(struct Sequencer* seq)
{
    sequencer_add_action(seq, fermer_porte_et_rentrer_ax, NULL, 1000);
}

void seq_ejecter_1_element(struct Sequencer* seq)
{
    sequencer_reset(seq);
    
    // 
    if ( (ax_caca_situation == POS_EJECTER_AX_CACA) && ((middle_place[TOB_INT]==1) || (middle_place[TOB_EXT]==1)) )
    {
        // AX pas en position, il faut le rentrer
        //sequencer_add_action(seq, fermer_porte_et_rentrer_ax, NULL, 1000);
        add_seq_fermer_porte_et_rentrer_ax(seq);
    }
    
    if (ax_caca_situation == POS_RENTRER_AX_CACA)
    {
        if ( (bottom_place[TOB_INT]==1) && (bottom_place[TOB_EXT]==1) )
        {
            sequencer_add_action(seq, servo_porte_ouvert, NULL, 50);
            sequencer_add_action(seq,  ax_caca_milieu, NULL, 300);
        }

        else if ( (bottom_place[TOB_INT]==1) || (bottom_place[TOB_EXT]==1) )
        {
            sequencer_add_action(seq, servo_porte_ouvert, NULL, 0);
            sequencer_add_action(seq,  ax_caca_ejecter, NULL, 400);
        }
    }
    else if (ax_caca_situation == POS_MILIEU_AX_CACA)
    {
        if (bottom_place[TOB_EXT]==1)
        {
            sequencer_add_action(seq, servo_porte_ouvert, NULL, 0);
            sequencer_add_action(seq,  ax_caca_ejecter, NULL, 400);
        }
    }
}

void seq_ejecter_2_element(struct Sequencer* seq)
{
    // on ne s'occupe pas de ça aujourd'hui.
}

// ============================================================================
// Système de séquences chaînées
// ============================================================================

/**
 * @brief Pointeur vers le séquenceur pour le chaînage
 *
 * Cette variable statique permet à l'action callback de relancer
 * la deuxième partie de la séquence après la première.
 */
static struct Sequencer* chained_sequencer = NULL;

/**
 * @brief Action callback : lance seq_ejecter_1_element après fermer_porte
 *
 * Cette action est appelée APRÈS que fermer_porte_et_rentrer_ax soit terminé.
 * Elle reconstruit la séquence avec les actions d'éjection, en réévaluant
 * l'état actuel du système (ax_caca_situation, bottom_place, etc.)
 *
 * @param param Non utilisé
 */
void action_launch_ejecter_sequence(void* param)
{
    (void)param;

    printf("[DBG] CB start\n");

    if (chained_sequencer == NULL) {
        printf("[ERR] seq=NULL\n");
        return;  // Sécurité : pas de séquenceur configuré
    }

    // NOTE: On ne fait PAS de sequencer_reset() ici car on est dans un callback
    // qui s'exécute PENDANT que le séquenceur est actif. Le reset écrasait les
    // données de la séquence en cours. La séquence parente se terminera naturellement.
    // sequencer_reset(chained_sequencer);  // ← COMMENTÉ pour éviter le bug

    // Maintenant que l'AX est rentré, réévaluer l'état et construire la séquence d'éjection
    // NOTE : ax_caca_situation a été mis à jour par fermer_porte_et_rentrer_ax()
    printf("[DBG] ax=%u I=%u E=%u\n",
           ax_caca_situation, bottom_place[TOB_INT], bottom_place[TOB_EXT]);

    if (ax_caca_situation == POS_RENTRER_AX_CACA)
    {
        if ((bottom_place[TOB_INT]==1) && (bottom_place[TOB_EXT]==1))
        {
            printf("[DBG] B1:2tob->mid\n");
            sequencer_add_action(chained_sequencer, servo_porte_ouvert, NULL, 150);
            sequencer_add_action(chained_sequencer, ax_caca_milieu, NULL, 200);
        }
        else if ((bottom_place[TOB_INT]==1) || (bottom_place[TOB_EXT]==1))
        {
            printf("[DBG] B2:1tob->eject\n");
            sequencer_add_action(chained_sequencer, servo_porte_ouvert, NULL, 0);
            sequencer_add_action(chained_sequencer, ax_caca_ejecter, NULL, 200);
        }
        else
        {
            printf("[DBG] B_empty\n");
        }
    }
    else if (ax_caca_situation == POS_MILIEU_AX_CACA)
    {
        printf("[DBG] AX=mid\n");
        if (bottom_place[TOB_EXT]==1)
        {
            printf("[DBG] B3:ext->eject\n");
            sequencer_add_action(chained_sequencer, servo_porte_ouvert, NULL, 0);
            sequencer_add_action(chained_sequencer, ax_caca_ejecter, NULL, 200);
        }
        else
        {
            printf("[DBG] B_ext_empty\n");
        }
    }
    else
    {
        printf("[ERR] ax=%u ?\n", ax_caca_situation);
    }

    // Redémarrer le séquenceur avec les nouvelles actions
    printf("[DBG] seq_start\n");
    printf("[DBG]_ ax=%u I=%u E=%u\n",
        ax_caca_situation, bottom_place[TOB_INT], bottom_place[TOB_EXT]);

    print_state_tobotan();
    sequencer_start(chained_sequencer);

    // Réinitialiser le pointeur (optionnel, pour sécurité)
    chained_sequencer = NULL;
}

/**
 * @brief Séquence combinée : ferme la porte, rentre l'AX, puis éjecte 1 élément
 *
 * Cette fonction construit une séquence en deux phases :
 * 1. Fermer porte et rentrer AX (si nécessaire)
 * 2. Callback qui relance automatiquement la séquence d'éjection
 *
 * L'avantage de cette approche : la deuxième phase réévalue l'état du système
 * APRÈS l'exécution de la première phase, ce qui garantit la cohérence.
 *
 * @param seq Séquenceur à remplir
 */
void seq_fermer_puis_ejecter_1_element(struct Sequencer* seq)
{
    sequencer_reset(seq);

    printf("[DBG] ===FE1 START===\n");
    printf("[DBG] ax=%u mI=%u mE=%u\n",
           ax_caca_situation, middle_place[TOB_INT], middle_place[TOB_EXT]);

    // Mémoriser le séquenceur pour le callback
    chained_sequencer = seq;

    // Phase 1 : Si AX en position éjecter, le rentrer d'abord
    if ((ax_caca_situation == POS_EJECTER_AX_CACA) &&
        ((middle_place[TOB_INT]==1) || (middle_place[TOB_EXT]==1)))
    {
        printf("[DBG] AX=ej+elem->close\n");
        sequencer_add_action(seq,servo_porte_fermer,NULL,300);
        sequencer_add_action(seq, fermer_porte_et_rentrer_ax, NULL, 500);

        // Action callback qui relancera la phase 2
        sequencer_add_action(seq, action_launch_ejecter_sequence, NULL, 500);
    }
    else
    {
        printf("[DBG] AX=in->direct\n");
        sequencer_add_action(seq, action_launch_ejecter_sequence, NULL, 0);
    }
    printf("[DBG] ===FE1 END===\n");
}