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
 * @brief IDs de commandes définissant les actions robot
 */
typedef enum {
    CMD_IDLE = 0,           /**< Aucune action */
    CMD_HOMING = 1,         /**< Séquence de homing complet */
    CMD_GRAB = 2,           /**< Séquence de saisie d'objet */
    CMD_DEPOSIT = 3,        /**< Séquence de dépôt d'objet */
    CMD_EMERGENCY_STOP = 99 /**< Arrêt d'urgence */
} CommandID_t;

/**
 * @brief Initialise le processeur de commandes CANopen
 *
 * À appeler une seule fois au démarrage, après l'initialisation du séquenceur.
 *
 * @param sequencer Pointeur vers le séquenceur principal
 */
void canopen_cmd_init(struct Sequencer* sequencer);

/**
 * @brief Traite les commandes CANopen reçues
 *
 * À appeler dans la boucle principale (while). Surveille les changements
 * de command_ID dans OD_RAM et déclenche les séquences correspondantes.
 */
void canopen_cmd_process(void);

#ifdef __cplusplus
}
#endif

#endif /* CANOPEN_COMMAND_PROCESSOR_H */
