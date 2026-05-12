/**
 * @file canopen_command_processor.c
 * @brief Implémentation du traitement des commandes CANopen
 */

#include "canopen_command_processor.h"
#include "CO_app_STM32.h"  // Pour canopenNodeSTM32
#include <stdio.h>
#include "ax_controller.h"
#include "robot_action_autom.h"
#include "i2c_servo_moteur.h"
#include "robot_data.h"    // Pour motor_drive, motor_R, motor_L
#include "stm32g4xx_hal.h" // Pour HAL_GetTick, __disable_irq, __NOP

/* Pointeur vers le séquenceur principal (initialisé par canopen_cmd_init) */
static struct Sequencer* main_sequencer = NULL;

/* Mémorisation du dernier command_ID traité pour détecter les changements */
static uint16_t last_command_id = 0xFFFF;

/* Flag pour suivre si le séquenceur était en cours d'exécution */
static uint8_t sequencer_was_running = 0;

/* Mémorisation de l'action et commande en cours */
static uint16_t current_action_id = 0;
static uint16_t current_cmd_id = 0;

/* Timer de match - Arrêt d'urgence après timeout */
static uint32_t match_start_tick = 0;  // 0 = match pas démarré
static const uint32_t MATCH_DURATION_MS = 99700;  // Durée du match en millisecondes

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

    printf("[CANopen STATUS] status=%u cmd_id=%u action_id=%u completed=%u error=%u (TPDO sent)\n",
           status, command_id, action_id,OD_RAM.x2103_last_completed_command_id, error_code);
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

    // ========== VÉRIFICATION TIMEOUT MATCH ==========
    if (match_start_tick != 0) {
        uint32_t current_tick = HAL_GetTick();
        uint32_t elapsed_ms = current_tick - match_start_tick;

        if (elapsed_ms >= MATCH_DURATION_MS) {
            printf("[MATCH TIMEOUT] Match duration exceeded (%lu ms)!\n", elapsed_ms);
            canopen_cmd_emergency_stop(CMD_ERROR_TIMEOUT);
            // Ne revient jamais ici (boucle infinie dans emergency_stop)
        }
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
                
                //seq_build_deposit(main_sequencer);
                enchement_seq_depose(main_sequencer);
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
            
            case CMD_POS_AX:
                printf("[CANopen CMD] Executing: AX\n\r");
                //canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                uint8_t id_ax = param_1;
                uint16_t pos_ax = param_2 ;

                ax_write_position(id_ax, pos_ax);
                //sequencer_was_running = 1;
                canopen_update_status(CMD_STATUS_COMPLETED, action_id, command_id, CMD_ERROR_NONE);
                break;
            
            case CMD_POMP_ON_OFF:
            { 
                printf("[CANopen CMD] Executing: CMD_POMP_ON_OFF\n\r");
                //canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                uint8_t on_off = param_1;
                if (on_off) 
                {
                    turn_on_pump_4(NULL);
                    turn_on_pump_3(NULL);
                }
                else
                {
                    turn_off_pump_4(NULL);
                    turn_off_pump_3(NULL);
                }

                canopen_update_status(CMD_STATUS_COMPLETED, action_id, command_id, CMD_ERROR_NONE);
                break;
            }

            case CMD_OPEN_PINCE:
                printf("[CANopen CMD] Executing: CMD_OPEN_PINCE\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_open_pince(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;

            case CMD_EJECTER:
                printf("[CANopen CMD] Executing: CMD_EJECTER\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                int num_element_a_ejecter = param_1;

                if (num_element_a_ejecter == 1)
                {
                    // Utilise la nouvelle séquence chaînée qui gère automatiquement
                    // le fermer_porte + rentrer_ax + éjecter dans le bon ordre
                    seq_fermer_puis_ejecter_1_element(main_sequencer);
                    sequencer_start(main_sequencer);
                    sequencer_was_running = 1;
                }
                else 
                {
                    canopen_update_status(CMD_STATUS_COMPLETED, action_id, command_id, CMD_ERROR_NONE);
                }

                break;

            case CMD_I2C_SERVO_MOTEUR:
                printf("[CANopen CMD] Executing: CMD_I2C_SERVO_MOTEUR\n\r");
                //canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);

                int cannal = param_1;
                int position = param_2; // valeur entre 500 et 2500
                i2c_servo(cannal, position);
                canopen_update_status(CMD_STATUS_COMPLETED, action_id, command_id, CMD_ERROR_NONE);
                break;
            
            case CDM_CLOSE_PINCE:
                printf("[CANopen CMD] Executing: CDM_CLOSE_PINCE\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_close_pince(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;
            
            case CDM_READY_TO_GRAP:
                printf("[CANopen CMD] Executing: CDM_READY_TO_GRAP\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_ready_to_grap(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;

            case CDM_SAFE_POSITION_ASCENSEUR:
                printf("[CANopen CMD] Executing: CDM_SAFE_POSITION_ASCENSEUR\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_safe_position(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;

            case CMD_COULEUR_EQUIPE:
                printf("[CANopen CMD] Executing: CMD_COULEUR_EQUIPE\n\r");
                uint8_t bleu_jaune = param_1;
                couleur_equipe = bleu_jaune ;
                canopen_update_status(CMD_STATUS_COMPLETED, action_id, command_id, CMD_ERROR_NONE);
                break;

            case CMD_FERMER_PORTE_RENTRER_AX_CACA:
                printf("[CANopen CMD] Executing: CMD_FERMER_PORTE_RENTRER_AX_CACA\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_fermer_porte_et_rentrer_ax(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;

            case CMD_POS_VITESSE_AX :
            { 
                printf("[CANopen CMD] Executing: CMD_POS_VITESSE_AX\n\r");
                uint8_t id_ax = param_1;
                uint16_t pos_ax = param_2 ;
                uint16_t vitess_ax = param_3 ;

                ax_write_position_and_speed(id_ax, pos_ax, vitess_ax);
                //sequencer_was_running = 1;
                canopen_update_status(CMD_STATUS_COMPLETED, action_id, command_id, CMD_ERROR_NONE);
                break;
            }

            case CMD_POS_AX_CACA_CALAGE:
            {
                printf("[CANopen CMD] Executing: CMD_POS_AX_CACA_CALAGE\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_ax_safe_pos_for_calage(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;
            }

            case CMD_AX_CURSOR_OUVERT : 
            {
                printf("[CANopen CMD] Executing: CMD_AX_CURSOR_OUVERT\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_ax_open_cursor(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;
            }

            case CMD_AX_CURSOR_FERMER:
            {
                printf("[CANopen CMD] Executing: CMD_AX_CURSOR_FERMER\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_ax_fermer_cursor(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;
            }

            case CMD_AX_GRAB_PINCE:
            {
                printf("[CANopen CMD] Executing: CMD_AX_GRAB_PINCE\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_grap_pince(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;
            }

            case CMD_OPEN_CURSOR_2:
            {
                printf("[CANopen CMD] Executing: CMD_OPEN_CURSOR_2\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_open_cursor_2(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;
            }

            case CMD_CLOSE_CURSOR_2:
            {
                printf("[CANopen CMD] Executing: CMD_CLOSE_CURSOR_2\n\r");
                canopen_update_status(CMD_STATUS_RUNNING, action_id, command_id, CMD_ERROR_NONE);
                seq_fermeture_cursor_2(main_sequencer);
                sequencer_start(main_sequencer);
                sequencer_was_running = 1;
                break;
            }

            case CMD_START_MATCH:
            {
                printf("[CANopen CMD] Executing: START_MATCH\n");

                // Enregistrer le tick actuel (horloge système)
                match_start_tick = HAL_GetTick();

                printf("Match started at tick: %lu\n", match_start_tick);

                // Signaler la commande comme complétée immédiatement
                canopen_update_status(CMD_STATUS_COMPLETED, action_id, command_id, CMD_ERROR_NONE);
                break;
            }

            case CMD_EMERGENCY_STOP:
                printf("[CANopen CMD] EMERGENCY STOP received via CAN!\n");
                canopen_cmd_emergency_stop(CMD_ERROR_EMERGENCY_STOP);
                // Ne revient jamais ici (boucle infinie dans emergency_stop)
                break;

            case CMD_IDLE:
                printf("[CANopen CMD] IDLE - No action\n");
                //canopen_update_status(CMD_STATUS_IDLE, 0, 0, CMD_ERROR_NONE);
                break;

            default:
                // Commande inconnue = ERREUR
                printf("[CANopen CMD] Unknown action_id: %u\n", action_id);
                canopen_update_status(CMD_STATUS_ERROR, action_id, command_id, CMD_ERROR_INVALID_COMMAND);
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

        sequencer_was_running = 0;
    }
}

// ============================================================================
// Notification de séquences manuelles (hors RPDO)
// ============================================================================

/**
 * @brief Signale qu'une séquence manuelle a démarré (homing initial, etc.)
 */
void canopen_signal_sequence_started(uint16_t action_id, uint16_t command_id)
{
    sequencer_was_running = 1;
    current_action_id = action_id;
    current_cmd_id = command_id;
    printf("[CANopen CMD] Manual sequence started: action_id=%u, cmd_id=%u\n",
           action_id, command_id);
}

// ============================================================================
// Arrêt d'urgence (Emergency Stop)
// ============================================================================

/**
 * @brief Arrêt d'urgence complet avec blocage du STM32
 *
 * Cette fonction arrête immédiatement tous les moteurs, signale l'erreur
 * via CANopen, puis bloque complètement le STM32 (désactivation des interruptions
 * + boucle infinie). Seul un reset physique peut redémarrer le système.
 *
 * @param error_code Code d'erreur à signaler (CMD_ERROR_TIMEOUT ou CMD_ERROR_EMERGENCY_STOP)
 */
void canopen_cmd_emergency_stop(uint8_t error_code)
{
    printf("[EMERGENCY STOP] Stopping all systems!\n");

    // 1. Arrêter les moteurs immédiatement
    motor_drive(motor_R, 0.0f);
    motor_drive(motor_L, 0.0f);

    // 2. Arrêter le séquenceur
    if (main_sequencer != NULL) {
        sequencer_reset(main_sequencer);
        sequencer_was_running = 0;
    }

    // 3. Signaler l'erreur via CANopen
    canopen_update_status(CMD_STATUS_ABORTED, current_action_id, current_cmd_id, error_code);

    printf("[EMERGENCY STOP] System halted - Reset required!\n");

    // 4. BLOCAGE TOTAL : désactiver toutes les interruptions
    __disable_irq();

    // 5. Boucle infinie - le STM32 est gelé
    while(1) {
        __NOP();  // No Operation
    }
}
