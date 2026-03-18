#ifndef __SEQUENCER_H
#define __SEQUENCER_H

/**
 * @file sequencer.h
 *
 * @brief Séquenceur générique : exécute une QUEUE d'étapes (moteurs + actions custom) de façon séquentielle.
 *
 * ## Principe
 *
 * Le séquenceur orchestre plusieurs MvStateMachine indépendantes (ex: moteur H et V).
 * Quand une étape moteur se termine, la machine repasse automatiquement en état IDLE (HOLD).
 * Cela permet d'avoir un moteur qui bouge pendant que l'autre reste en position.
 *
 * ## Types d'étapes
 *
 * 1. **SEQ_STEP_MOTOR** : Envoie un état à une MvStateMachine, attend la fin du mouvement
 * 2. **SEQ_STEP_ACTION** : Exécute un callback custom immédiatement (GPIO, UART, I2C, délai...)
 *
 * ## Exemple complet
 *
 * ```c
 * // === 1. INITIALISATION (une seule fois) ===
 * struct Sequencer seq = sequencer_init();
 *
 * // Enregistrer les états IDLE pour chaque moteur
 * sequencer_set_idle(&seq, &elevator_H_statemachine, &ELV_STATE_HOLD_H, env_hold_h);
 * sequencer_set_idle(&seq, &elevator_V_statemachine, &ELV_STATE_HOLD_V, env_hold_v);
 *
 * // === 2. CRÉER VOS FONCTIONS CALLBACK ===
 * void my_gpio_set(void* param) {
 *     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
 * }
 *
 * void my_uart_send(void* param) {
 *     HAL_UART_Transmit(&huart3, "OK", 2, 100);
 * }
 *
 * // === 3. CONSTRUIRE LA SÉQUENCE ===
 * sequencer_reset(&seq);  // Vider la queue
 *
 * // Étape 1 : Mouvement horizontal
 * sequencer_add(&seq, &elevator_H_statemachine, &ELV_STATE_TRANSLATION_H, env_h);
 * // → Moteur H bouge, moteur V reste en HOLD
 * // → À la fin, moteur H repasse en HOLD automatiquement
 *
 * // Étape 2 : GPIO
 * sequencer_add_action(&seq, my_gpio_set, NULL);
 *
 * // Étape 3 : Mouvement vertical
 * sequencer_add(&seq, &elevator_V_statemachine, &ELV_STATE_TRANSLATION_V, env_v);
 * // → Moteur V bouge, moteur H reste en HOLD
 *
 * // Étape 4 : UART
 * sequencer_add_action(&seq, my_uart_send, NULL);
 *
 * // === 4. LANCER LA SÉQUENCE ===
 * sequencer_start(&seq);
 *
 * // === 5. DANS LA BOUCLE PRINCIPALE (ou timer interrupt) ===
 * while(1) {
 *     sequencer_update(&seq);  // Avance la séquence automatiquement
 *     HAL_Delay(10);
 * }
 * ```
 */

#include "main.h"
#include "mv_statemachine.h"

#define SEQUENCER_MAX_STEPS    16
#define SEQUENCER_MAX_MACHINES  4


/**
 * @brief Type d'étape dans la séquence
 */
enum SeqStepType
{
    SEQ_STEP_MOTOR,    // Étape moteur (attend fin d'état machine)
    SEQ_STEP_ACTION    // Étape action (callback immédiat)
};


/**
 * @brief Une étape de séquence : soit un mouvement moteur, soit une action.
 *
 * Utilise une union pour économiser la mémoire : une étape est SOIT un mouvement,
 * SOIT une action, jamais les deux en même temps.
 */
struct SeqStep
{
    enum SeqStepType type;  // Discriminant : indique quel membre de l'union utiliser

    union {
        // Option 1 : Étape moteur (type == SEQ_STEP_MOTOR)
        struct {
            struct MvStateMachine* machine;
            struct MvState*        state;
            struct MvStateEnv      env;
        } motor;

        // Option 2 : Étape action (type == SEQ_STEP_ACTION)
        struct {
            void (*callback)(void* param);  // Fonction à exécuter
            void* param;                     // Paramètres de la fonction
            uint32_t delay_after_ms;         // Délai à attendre APRÈS l'exécution (0 = pas de délai)
            uint32_t elapsed_time_ms;        // Temps écoulé depuis l'exécution (géré par sequencer_update)
            uint8_t  executed;               // Flag: 1 = callback déjà exécuté, attend le délai
        } action;
    };
};

/**
 * @brief État idle d'une machine : chargé automatiquement quand une étape se termine.
 */
struct SeqIdle
{
    struct MvStateMachine* machine;
    struct MvState*        state;
    struct MvStateEnv      env;
};


/**
 * @brief Le séquenceur : liste ordonnée d'étapes + états idle par machine.
 *
 * Le séquenceur permet d'exécuter une QUEUE d'étapes de façon séquentielle.
 * Chaque étape peut être :
 * - Un mouvement moteur (SEQ_STEP_MOTOR) : attend la fin du mouvement
 * - Une action custom (SEQ_STEP_ACTION) : exécute un callback immédiatement
 *
 * Exemple de séquence :
 * 1. Bouger moteur H → HOLD automatique après
 * 2. GPIO haut
 * 3. Bouger moteur V → HOLD automatique après
 * 4. Commande UART
 */
struct Sequencer
{
    // ========== QUEUE D'ÉTAPES ==========

    struct SeqStep  steps[SEQUENCER_MAX_STEPS];  // Liste des étapes à exécuter (max 16)
    uint8_t         step_count;                  // Nombre total d'étapes dans la queue (combien d'étapes ajoutées)
    uint8_t         current_step;                // Index de l'étape en cours d'exécution (0 à step_count-1)

    // ========== ÉTAT D'EXÉCUTION ==========

    uint8_t         active;       // 1 = séquence en cours, 0 = terminée ou pas encore démarrée
    uint8_t         step_sent;    // 1 = l'étape courante a été envoyée à la machine (évite double-envoi)

    // ========== ÉTATS IDLE (HOLD) ==========

    struct SeqIdle  idles[SEQUENCER_MAX_MACHINES];  // États idle pour chaque machine (ex: HOLD)
    uint8_t         idle_count;                     // Nombre de machines enregistrées (max 4)
};


/**
 * @brief Initialise un séquenceur vide.
 */
struct Sequencer sequencer_init(void);

/**
 * @brief Enregistre l'état idle (HOLD) d'une machine.
 *        Appelé une fois à l'init, avant sequencer_start().
 *        Quand une étape sur cette machine se termine, elle repasse automatiquement en idle.
 */
void sequencer_set_idle(struct Sequencer* seq,
                        struct MvStateMachine* machine,
                        struct MvState* idle_state,
                        struct MvStateEnv idle_env);

/**
 * @brief Ajoute une étape MOTEUR à la fin de la séquence.
 * @return 0 si succès, -1 si la séquence est pleine.
 */
int sequencer_add(struct Sequencer* seq,
                  struct MvStateMachine* machine,
                  struct MvState* state,
                  struct MvStateEnv env);

/**
 * @brief Ajoute une étape ACTION à la fin de la séquence.
 *        L'action sera exécutée immédiatement quand son tour arrive.
 * @param callback        Fonction à appeler (signature: void callback(void* param))
 * @param param           Paramètre à passer à la fonction (peut être NULL)
 * @param delay_after_ms  Délai NON-BLOQUANT à attendre APRÈS l'exécution (0 = pas de délai)
 * @return 0 si succès, -1 si la séquence est pleine.
 *
 * Exemple :
 *   sequencer_add_action(seq, ax_servo_6_close, NULL, 5);  // Exécute puis attend 5ms
 *   sequencer_add_action(seq, ax_servo_7_close, NULL, 0);  // Exécute immédiatement
 */
int sequencer_add_action(struct Sequencer* seq,
                         void (*callback)(void* param),
                         void* param,
                         uint32_t delay_after_ms);

/**
 * @brief Démarre l'exécution de la séquence depuis le début.
 */
void sequencer_start(struct Sequencer* seq);

/**
 * @brief Remet le séquenceur à zéro (vide les étapes, conserve les idles).
 */
void sequencer_reset(struct Sequencer* seq);

/**
 * @brief À appeler à chaque cycle dans le timer interrupt.
 *        Avance la séquence quand la machine courante a terminé son état.
 *        Remet automatiquement en idle toute machine qui vient de terminer.
 * @param seq             Séquenceur à mettre à jour
 * @param delta_time_ms   Temps écoulé depuis le dernier appel en millisecondes
 */
void sequencer_update(struct Sequencer* seq, uint32_t delta_time_ms);

/**
 * @brief Indique si la séquence est en cours d'exécution.
 */
uint8_t sequencer_is_active(const struct Sequencer* seq);


#endif // __SEQUENCER_H
