/**
 * @file canopen_command_processor.h
 * @brief Traitement des commandes CANopen reçues via RPDO
 *
 * Ce module surveille les changements dans l'Object Dictionary (OD_RAM)
 * et déclenche les séquences robot correspondantes.
 */

#ifndef CANOPEN_COMMAND_PROCESSOR_H
#define CANOPEN_COMMAND_PROCESSOR_H

#include "sequencer.h"
#include "robot_sequences.h"
#include "CANopen.h"  // Must be included before OD.h for OD_t type
#include "OD.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief IDs de commandes définissant les actions robot (synchronisé avec OD 0x2000)
 */
typedef enum {
    CMD_IDLE = 0,           /**< Aucune action */
    CMD_HOMING = 1,         /**< Séquence de homing complet */
    CMD_GRAB = 2,           /**< Séquence de saisie d'objet */
    CMD_DEPOSIT = 3,        /**< Séquence de dépôt d'objet */
    CMD_POS_ELAVATOR_H = 4,
    CMD_POS_ELAVATOR_V = 5,
    CMD_POS_AX = 6,
    CMD_POMP_ON_OFF = 7,
    CMD_OPEN_PINCE = 8,
    CMD_EJECTER = 9,
    CMD_I2C_SERVO_MOTEUR = 10,
    CDM_CLOSE_PINCE = 11,
    CDM_READY_TO_GRAP = 12,
    CDM_SAFE_POSITION_ASCENSEUR = 13,
    CMD_COULEUR_EQUIPE = 14,
    CMD_FERMER_PORTE_RENTRER_AX_CACA = 15,
    CMD_POS_VITESSE_AX = 16,
    CMD_POS_AX_CACA_CALAGE= 17,
    CMD_AX_CURSOR_OUVERT = 18,
    CMD_AX_CURSOR_FERMER = 19,
    CMD_AX_GRAB_PINCE = 20,
    CMD_EMERGENCY_STOP = 99 /**< Arrêt d'urgence */
} CommandID_t;

/**
 * @brief États possibles d'une commande (synchronisé avec OD 0x2100)
 */
typedef enum {
    CMD_STATUS_IDLE = 0,      /**< Prêt à recevoir une nouvelle commande */
    CMD_STATUS_RUNNING = 1,   /**< Commande en cours d'exécution */
    CMD_STATUS_COMPLETED = 2, /**< Commande terminée avec succès */
    CMD_STATUS_ERROR = 3,     /**< Erreur pendant l'exécution */
    CMD_STATUS_ABORTED = 4    /**< Commande annulée (arrêt d'urgence) */
} CommandStatus_t;

/**
 * @brief Codes d'erreur possibles (synchronisé avec OD 0x2104)
 */
typedef enum {
    CMD_ERROR_NONE = 0,               /**< Aucune erreur */
    CMD_ERROR_INVALID_COMMAND = 1,    /**< ACTION_ID inconnu */
    CMD_ERROR_INVALID_PARAMS = 2,     /**< Paramètres invalides */
    CMD_ERROR_TIMEOUT = 3,            /**< Timeout lors de l'exécution */
    CMD_ERROR_MECHANICAL = 4,         /**< Erreur mécanique (blocage, fin de course) */
    CMD_ERROR_EMERGENCY_STOP = 99     /**< Arrêt d'urgence déclenché */
} CommandErrorCode_t;

/**
 * @brief Initialise le processeur de commandes CANopen
 *
 * À appeler une seule fois au démarrage, après l'initialisation du séquenceur.
 *
 * @param sequencer Pointeur vers le séquenceur principal
 */
void canopen_cmd_init(struct Sequencer* sequencer);

/**
 * @brief Lit une commande depuis l'Object Dictionary (RPDO → STM32)
 *
 * @param action_id    Pointeur pour stocker l'ACTION_ID lu (peut être NULL)
 * @param command_id   Pointeur pour stocker le command_ID lu (peut être NULL)
 * @param param_1      Pointeur pour stocker le param_1 (peut être NULL)
 * @param param_2      Pointeur pour stocker le param_2 (peut être NULL)
 * @param param_3      Pointeur pour stocker le param_3 (peut être NULL)
 * @param param_4      Pointeur pour stocker le param_4 (peut être NULL)
 * @param param_5      Pointeur pour stocker le param_5 (peut être NULL)
 * @param param_6      Pointeur pour stocker le param_6 (peut être NULL)
 */
void canopen_read_command(uint16_t* action_id,
                          uint16_t* command_id,
                          int16_t* param_1,
                          int16_t* param_2,
                          int16_t* param_3,
                          int16_t* param_4,
                          int16_t* param_5,
                          int16_t* param_6);

/**
 * @brief Met à jour le statut dans l'Object Dictionary et déclenche un TPDO (STM32 → CANopen)
 *
 * @param status       Statut actuel (voir CommandStatus_t)
 * @param action_id    ACTION_ID en cours d'exécution
 * @param command_id   command_ID en cours d'exécution
 * @param error_code   Code d'erreur (voir CommandErrorCode_t)
 */
void canopen_update_status(uint8_t status,
                          uint16_t action_id,
                          uint16_t command_id,
                          uint8_t error_code);

/**
 * @brief Traite les commandes CANopen reçues
 *
 * À appeler dans la boucle principale (while). Surveille les changements
 * de command_ID dans OD_RAM et déclenche les séquences correspondantes.
 */
void canopen_cmd_process(void);

/**
 * @brief Signale qu'une séquence manuelle a démarré (homing initial, etc.)
 *
 * Cette fonction permet de notifier le système CANopen qu'une séquence
 * a été lancée manuellement (hors RPDO), afin qu'il puisse détecter
 * automatiquement sa fin et passer en statut IDLE.
 *
 * @param action_id  ID de l'action en cours (ex: CMD_HOMING)
 * @param command_id ID de commande fictif (peut être 0 pour homing au démarrage)
 */
void canopen_signal_sequence_started(uint16_t action_id, uint16_t command_id);

#ifdef __cplusplus
}
#endif

#endif /* CANOPEN_COMMAND_PROCESSOR_H */
