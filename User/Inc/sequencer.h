#ifndef __SEQUENCER_H
#define __SEQUENCER_H

/**
 * @file sequencer.h
 *
 * @brief Séquenceur de mouvements : exécute une séquence d'étapes sur une ou plusieurs
 *        MvStateMachine de façon séquentielle.
 *
 * Chaque machine a un état "idle" (typiquement HOLD) qui est chargé automatiquement
 * dès qu'une étape se termine. Ainsi, les moteurs sont toujours maintenus en position.
 *
 * Exemple :
 *
 *   sequencer_set_idle_V(&seq, &ELV_STATE_HOLD_V, genenv_elv_hold_v());
 *   sequencer_set_idle_H(&seq, &ELV_STATE_HOLD_H, genenv_elv_hold_h());
 *
 *   sequencer_add(&seq, &elevator_V_statemachine, &ELV_STATE_HOME_V, genenv_elv_home_v(-20.0f));
 *   sequencer_add(&seq, &elevator_H_statemachine, &ELV_STATE_HOME_H, genenv_elv_home_h(-10.0f));
 *
 *   sequencer_start(&seq);
 *
 *   // Dans le timer interrupt :
 *   sequencer_update(&seq);
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
 */
struct Sequencer
{
    struct SeqStep  steps[SEQUENCER_MAX_STEPS];
    uint8_t         step_count;
    uint8_t         current_step;
    uint8_t         active;       // 1 = en cours, 0 = terminé ou pas encore démarré
    uint8_t         step_sent;    // 1 = l'étape courante a été envoyée à la machine

    struct SeqIdle  idles[SEQUENCER_MAX_MACHINES];
    uint8_t         idle_count;
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
 * @param callback  Fonction à appeler (signature: void callback(void* param))
 * @param param     Paramètre à passer à la fonction (peut être NULL)
 * @return 0 si succès, -1 si la séquence est pleine.
 */
int sequencer_add_action(struct Sequencer* seq,
                         void (*callback)(void* param),
                         void* param);

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
 */
void sequencer_update(struct Sequencer* seq);

/**
 * @brief Indique si la séquence est en cours d'exécution.
 */
uint8_t sequencer_is_active(const struct Sequencer* seq);


// ============================================================================
// Helpers pour actions courantes
// ============================================================================

/**
 * @brief Paramètres pour action GPIO
 */
struct SeqGpioAction
{
    GPIO_TypeDef* port;
    uint16_t pin;
    GPIO_PinState state;  // GPIO_PIN_SET ou GPIO_PIN_RESET
};

/**
 * @brief Paramètres pour action délai
 */
struct SeqDelayAction
{
    uint32_t delay_ms;
    uint32_t start_tick;   // Rempli automatiquement
};

/**
 * @brief Ajoute une action GPIO (mise à l'état haut ou bas d'une pin)
 * @param port   Port GPIO (ex: GPIOA)
 * @param pin    Pin GPIO (ex: GPIO_PIN_5)
 * @param state  État à appliquer (GPIO_PIN_SET ou GPIO_PIN_RESET)
 * @return 0 si succès, -1 si la séquence est pleine.
 *
 * ATTENTION : Les paramètres GPIO sont copiés dans un buffer statique interne.
 *             Limite : 8 actions GPIO maximum dans une séquence.
 */
int sequencer_add_gpio(struct Sequencer* seq,
                       GPIO_TypeDef* port,
                       uint16_t pin,
                       GPIO_PinState state);

/**
 * @brief Ajoute un délai (pause) dans la séquence
 * @param delay_ms  Durée du délai en millisecondes
 * @return 0 si succès, -1 si la séquence est pleine.
 *
 * ATTENTION : Le délai utilise HAL_GetTick() et bloque l'avancement de la séquence.
 *             Limite : 8 délais maximum dans une séquence.
 */
int sequencer_add_delay(struct Sequencer* seq, uint32_t delay_ms);


#endif // __SEQUENCER_H
