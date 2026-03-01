/**
 * @file canopen_command_processor.c
 * @brief Implémentation du traitement des commandes CANopen
 */

#include "canopen_command_processor.h"
#include "CO_app_STM32.h"  // Pour canopenNodeSTM32
#include <stdio.h>

/* Pointeur vers le séquenceur principal (initialisé par canopen_cmd_init) */
static struct Sequencer* main_sequencer = NULL;

/* Mémorisation du dernier command_ID traité pour détecter les changements */
static uint16_t last_command_id = 0xFFFF;

/* Flag pour suivre si le séquenceur était en cours d'exécution */
static uint8_t sequencer_was_running = 0;

/* Mémorisation de l'action et commande en cours */
static uint16_t current_action_id = 0;
static uint16_t current_cmd_id = 0;

/* Référence externe au stack CANopen */
extern CANopenNodeSTM32* canopenNodeSTM32;

/**
 * @brief Initialise le processeur de commandes CANopen
 */
void canopen_cmd_init(struct Sequencer* sequencer) {
    main_sequencer = sequencer;
    printf("[CANopen CMD] Processor initialized\n");

    // Initialise le statut CANopen à IDLE
    canopen_update_status(CMD_STATUS_IDLE, 0, 0, CMD_ERROR_NONE);
}

// ============================================================================
// Lecture des commandes CANopen (RPDO → STM32)
// ============================================================================

/**
 * @brief Lit une commande depuis l'Object Dictionary (RPDO → STM32)
 */
void canopen_read_command(uint16_t* action_id,
                          uint16_t* command_id,
                          int16_t* param_1,
                          int16_t* param_2,
                          int16_t* param_3,
                          int16_t* param_4,
                          int16_t* param_5,
                          int16_t* param_6)
{
    // DEBUG: Afficher les valeurs brutes de l'OD avant copie
    static uint16_t last_logged_cmd_id = 0xFFFF;
    uint16_t current_cmd_id = OD_RAM.x2007_command_ID;

    if (current_cmd_id != last_logged_cmd_id) {
        printf("[OD_READ] RAW OD values: ACTION_ID=0x%04X CMD_ID=0x%04X p1=%d p2=%d p3=%d p4=%d p5=%d p6=%d\n",
               OD_RAM.x2000_ACTION_ID, OD_RAM.x2007_command_ID,
               OD_RAM.x2001_param_1, OD_RAM.x2002_param_2, OD_RAM.x2003_param_3,
               OD_RAM.x2004_param_4, OD_RAM.x2005_param_5, OD_RAM.x2006_param_6);
        last_logged_cmd_id = current_cmd_id;
    }

    if (action_id != NULL)
        *action_id = OD_RAM.x2000_ACTION_ID;

    if (command_id != NULL)
        *command_id = OD_RAM.x2007_command_ID;

    if (param_1 != NULL)
        *param_1 = OD_RAM.x2001_param_1;

    if (param_2 != NULL)
        *param_2 = OD_RAM.x2002_param_2;

    if (param_3 != NULL)
        *param_3 = OD_RAM.x2003_param_3;

    if (param_4 != NULL)
        *param_4 = OD_RAM.x2004_param_4;

    if (param_5 != NULL)
        *param_5 = OD_RAM.x2005_param_5;

    if (param_6 != NULL)
        *param_6 = OD_RAM.x2006_param_6;
}

// ============================================================================
// Mise à jour du statut CANopen (STM32 → TPDO)
// ============================================================================

/**
 * @brief Met à jour le statut dans l'Object Dictionary et déclenche un TPDO
 */
void canopen_update_status(uint8_t status,
                          uint16_t action_id,
                          uint16_t command_id,
                          uint8_t error_code)
{
    // Mise à jour de l'Object Dictionary
    OD_RAM.x2100_current_command_status = status;
    OD_RAM.x2101_current_command_id = command_id;
    OD_RAM.x2102_current_action_id = action_id;
    OD_RAM.x2104_command_error_code = error_code;

    // Si la commande vient de se terminer avec succès, enregistrer dans last_completed
    if (status == CMD_STATUS_COMPLETED)
    {
        OD_RAM.x2103_last_completed_command_id = command_id;
    }

    // Déclenchement explicite du TPDO1 (événementiel)
    CO_TPDOsendRequest(&canopenNodeSTM32->canOpenStack->TPDO[0]);

    printf("[CANopen STATUS] status=%u action_id=%u cmd_id=%u error=%u (TPDO sent)\n",
           status, action_id, command_id, error_code);
}
// ============================================================================
// Traitement des commandes CANopen
// ============================================================================

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

    // ========== LECTURE COMMANDE (CANopen → STM32) ==========
    uint16_t action_id, command_id;
    int16_t param_1, param_2, param_3, param_4, param_5, param_6;

    canopen_read_command(&action_id, &command_id, &param_1, &param_2, &param_3, &param_4, &param_5, &param_6);

    // ========== DÉTECTION NOUVELLE COMMANDE ==========
    if (command_id != last_command_id && command_id != 0)
    {
        printf("[CANopen CMD] New command: action_id=%u, command_id=%u\n",
               action_id, command_id);

        // Mémorisation pour la détection de fin de séquence
        current_action_id = action_id;
        current_cmd_id = command_id;

        // ========== EXÉCUTION SELON L'ACTION_ID ==========
        switch (action_id)
        {
            case CMD_HOMING:
                printf("[CANopen CMD] Executing: HOMING\n");

                // 1. Signaler le démarrage (RUNNING)
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);

                // 2. Lecture des paramètres (avec valeurs par défaut)
                //float speed_V = (param_1 != 0) ? (float)param_1 : 10.0f;
                //float speed_H = (param_2 != 0) ? (float)param_2 : 10.0f;
                float speed_V = param_1;
                float speed_H = param_2;
                // 3. Lancement de la séquence
                seq_build_homing_all(main_sequencer, speed_V, speed_H);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;

            case CMD_GRAB:
                printf("[CANopen CMD] Executing: GRAB\n");

                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);

                seq_build_grab(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;

            case CMD_DEPOSIT:
                printf("[CANopen CMD] Executing: DEPOSIT\n");

                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);

                seq_build_deposit(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;

            case CMD_POS_ELAVATOR_H:
                printf("[CANopen CMD] Executing: CMD_POS_ELAVATOR_H\n");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                printf("___param_1%d\n\r",param_1);
                float pos_h_mm = (float)param_1 ;
                printf("_____pos_h_mm%f\n\r",pos_h_mm);
                seq_deplacement_H(main_sequencer,pos_h_mm);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;

            case CMD_POS_ELAVATOR_V:
                printf("[CANopen CMD] Executing: CMD_POS_ELAVATOR_V\n");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);

                float pos_v_mm = (float)param_1 ;
                seq_deplacement_V(main_sequencer,pos_v_mm);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;

            case CMD_EMERGENCY_STOP:
                printf("[CANopen CMD] EMERGENCY STOP!\n");

                // Arrêt immédiat : ABORTED
                canopen_update_status(CMD_STATUS_ABORTED, action_id, command_id, CMD_ERROR_EMERGENCY_STOP);

                sequencer_reset(main_sequencer);
                sequencer_was_running = 0;

                // Repasser en IDLE immédiatement
                canopen_update_status(CMD_STATUS_IDLE, 0, 0, CMD_ERROR_NONE);
                break;

            case CMD_IDLE:
                printf("[CANopen CMD] IDLE - No action\n");
                canopen_update_status(CMD_STATUS_IDLE, 0, 0, CMD_ERROR_NONE);
                break;

            default:
                // Commande inconnue = ERREUR
                printf("[CANopen CMD] Unknown action_id: %u\n", action_id);
                canopen_update_status(CMD_STATUS_ERROR, action_id, command_id, CMD_ERROR_INVALID_COMMAND);

                // Repasser en IDLE
                canopen_update_status(CMD_STATUS_IDLE, 0, 0, CMD_ERROR_NONE);
                break;
        }

        // Mémorisation de la commande traitée
        last_command_id = command_id;
    }

    // ========== DÉTECTION FIN DE SÉQUENCE ==========
    if (sequencer_was_running && !sequencer_is_active(main_sequencer))
    {
        printf("[CANopen CMD] Sequence completed for command_id=%u\n", current_cmd_id);

        // Signaler la fin (COMPLETED)
        canopen_update_status(CMD_STATUS_COMPLETED, current_action_id, current_cmd_id, CMD_ERROR_NONE);

        // Repasser en IDLE (prêt pour la prochaine commande)
        canopen_update_status(CMD_STATUS_IDLE, 0, 0, CMD_ERROR_NONE);

        sequencer_was_running = 0;
    }
}
