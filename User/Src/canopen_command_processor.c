/**
 * @file canopen_command_processor.c
 * @brief Implémentation du traitement des commandes CANopen
 */

#include "canopen_command_processor.h"
#include <stdio.h>

/* Pointeur vers le séquenceur principal (initialisé par canopen_cmd_init) */
static struct Sequencer* main_sequencer = NULL;

/* Mémorisation du dernier command_ID traité pour détecter les changements */
static uint16_t last_command_id = 0xFFFF;

/**
 * @brief Initialise le processeur de commandes CANopen
 */
void canopen_cmd_init(struct Sequencer* sequencer) {
    main_sequencer = sequencer;
    printf("[CANopen CMD] Processor initialized\n");
}

/**
 * @brief Traite les commandes CANopen reçues
 *
 * Cette fonction surveille command_ID (OD_RAM.x2007_command_ID).
 * Lorsqu'un changement est détecté, elle déclenche la séquence correspondante.
 */
void canopen_cmd_process(void) {
    if (main_sequencer == NULL) {
        return; // Pas encore initialisé
    }

    // Lecture du command_ID depuis l'Object Dictionary
    uint16_t current_cmd_id = OD_RAM.x2007_command_ID;

    // Détection de changement
    if (current_cmd_id != last_command_id) {
        printf("[CANopen CMD] New command received: %u\n", current_cmd_id);

        // Traitement selon le command_ID
        switch (current_cmd_id) {
            case CMD_HOMING:
                printf("[CANopen CMD] Executing: HOMING\n");
                // Lecture des paramètres depuis OD_RAM
                // param_1 = vitesse homing vertical (ou valeur par défaut)
                // param_2 = vitesse homing horizontal
                float speed_V = (OD_RAM.x2001_param_1 != 0) ? (float)OD_RAM.x2001_param_1 : 10.0f;
                float speed_H = (OD_RAM.x2002_param_2 != 0) ? (float)OD_RAM.x2002_param_2 : 10.0f;

                seq_build_homing_all(main_sequencer, speed_V, speed_H);
                sequencer_start(main_sequencer);
                break;

            case CMD_GRAB:
                printf("[CANopen CMD] Executing: GRAB\n");
                // Lecture des positions depuis OD_RAM
                // param_1 = position verticale (mm)
                // param_2 = position horizontale (mm)
                float grab_pos_V = (float)OD_RAM.x2001_param_1;
                float grab_pos_H = (float)OD_RAM.x2002_param_2;

                seq_build_grab(main_sequencer, grab_pos_V, grab_pos_H);
                sequencer_start(main_sequencer);
                break;

            case CMD_DEPOSIT:
                printf("[CANopen CMD] Executing: DEPOSIT\n");
                // Lecture des positions depuis OD_RAM
                float deposit_pos_V = (float)OD_RAM.x2001_param_1;
                float deposit_pos_H = (float)OD_RAM.x2002_param_2;

                seq_build_deposit(main_sequencer, deposit_pos_V, deposit_pos_H);
                sequencer_start(main_sequencer);
                break;

            case CMD_EMERGENCY_STOP:
                printf("[CANopen CMD] EMERGENCY STOP!\n");
                // Arrêt immédiat : reset séquenceur + arrêt des machines d'état
                sequencer_reset(main_sequencer);
                // Les machines repasseront en idle (HOLD) au prochain update
                break;

            case CMD_IDLE:
                printf("[CANopen CMD] IDLE - No action\n");
                break;

            default:
                printf("[CANopen CMD] Unknown command ID: %u\n", current_cmd_id);
                break;
        }

        // Mémorisation de la commande traitée
        last_command_id = current_cmd_id;
    }
}
