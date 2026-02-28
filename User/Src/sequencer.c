#include "sequencer.h"

#include <string.h>
#include <stdio.h>


// ============================================================================
// Variables statiques pour les buffers d'actions
// ============================================================================
// IMPORTANT : Ces variables doivent être déclarées AVANT sequencer_reset()
// car elles sont utilisées dans cette fonction.

#define SEQ_ACTION_BUFFER_SIZE 8
static struct SeqGpioAction  gpio_action_buffer[SEQ_ACTION_BUFFER_SIZE];
static struct SeqDelayAction delay_action_buffer[SEQ_ACTION_BUFFER_SIZE];
static uint8_t gpio_buffer_index = 0;
static uint8_t delay_buffer_index = 0;


// ============================================================================
// Fonctions internes
// ============================================================================

// Envoie un état à une machine (reset construction + enqueue + ready)
static void send_to_machine(struct MvStateMachine* m, struct MvState* state, struct MvStateEnv env)
{
    MSM_reset_construction(m);
    MSM_enqueue_state(m, state, env);
    MSM_ready_construction(m);
}

// Recherche l'idle enregistré pour une machine donnée, retourne NULL si non trouvé
static struct SeqIdle* find_idle(struct Sequencer* seq, struct MvStateMachine* machine)
{
    for (uint8_t i = 0; i < seq->idle_count; i++)
    {
        if (seq->idles[i].machine == machine)
            return &seq->idles[i];
    }
    return 0;
}


struct Sequencer sequencer_init(void)
{
    struct Sequencer seq;
    memset(&seq, 0, sizeof(seq));
    return seq;
}


void sequencer_set_idle(struct Sequencer* seq,
                        struct MvStateMachine* machine,
                        struct MvState* idle_state,
                        struct MvStateEnv idle_env)
{
    if (seq->idle_count >= SEQUENCER_MAX_MACHINES)
    {
        printf("sequencer: too many machines\n");
        return;
    }
    seq->idles[seq->idle_count].machine = machine;
    seq->idles[seq->idle_count].state   = idle_state;
    seq->idles[seq->idle_count].env     = idle_env;
    seq->idle_count++;
    printf("sequencer: idle registered for machine %d\n", seq->idle_count - 1);
}


int sequencer_add(struct Sequencer* seq,
                  struct MvStateMachine* machine,
                  struct MvState* state,
                  struct MvStateEnv env)
{
    if (seq->step_count >= SEQUENCER_MAX_STEPS)
    {
        printf("sequencer: full, cannot add step\n");
        return -1;
    }

    // Type MOTOR
    seq->steps[seq->step_count].type = SEQ_STEP_MOTOR;
    seq->steps[seq->step_count].motor.machine = machine;
    seq->steps[seq->step_count].motor.state   = state;
    seq->steps[seq->step_count].motor.env     = env;

    printf("sequencer: add MOTOR step %d\n", seq->step_count);
    seq->step_count++;
    return 0;
}


int sequencer_add_action(struct Sequencer* seq,
                         void (*callback)(void* param),
                         void* param)
{
    if (seq->step_count >= SEQUENCER_MAX_STEPS)
    {
        printf("sequencer: full, cannot add action\n");
        return -1;
    }

    // Type ACTION
    seq->steps[seq->step_count].type = SEQ_STEP_ACTION;
    seq->steps[seq->step_count].action.callback = callback;
    seq->steps[seq->step_count].action.param    = param;

    printf("sequencer: add ACTION step %d\n", seq->step_count);
    seq->step_count++;
    return 0;
}


void sequencer_start(struct Sequencer* seq)
{
    if (seq->step_count == 0)
        return;

    seq->current_step = 0;
    seq->active       = 1;
    seq->step_sent    = 0;
    printf("sequencer: start (%d steps)\n", seq->step_count);
}


void sequencer_reset(struct Sequencer* seq)
{
    // Conserve les idles, réinitialise uniquement les étapes
    uint8_t idle_count = seq->idle_count;
    struct SeqIdle idles[SEQUENCER_MAX_MACHINES];
    memcpy(idles, seq->idles, sizeof(idles));

    memset(seq, 0, sizeof(*seq));

    seq->idle_count = idle_count;
    memcpy(seq->idles, idles, sizeof(idles));

    // Réinitialise les buffers d'actions
    gpio_buffer_index = 0;
    delay_buffer_index = 0;
}


void sequencer_update(struct Sequencer* seq)
{
    if (!seq->active)
        return;

    if (seq->current_step >= seq->step_count)
    {
        seq->active = 0;
        printf("sequencer: ALL DONE (%d steps completed)\n", seq->step_count);
        return;
    }

    struct SeqStep* step = &seq->steps[seq->current_step];

    // ========== TRAITEMENT SELON LE TYPE D'ÉTAPE ==========

    if (step->type == SEQ_STEP_MOTOR)
    {
        // === ÉTAPE MOTEUR : Attente fin d'état machine ===
        struct MvStateMachine* m = step->motor.machine;

        // Envoie l'étape courante à la machine si ce n'est pas encore fait
        if (!seq->step_sent)
        {
            send_to_machine(m, step->motor.state, step->motor.env);
            seq->step_sent = 1;
            printf("sequencer: --> MOTOR step %d/%d sent\n",
                   seq->current_step + 1, seq->step_count);
            return;  // attend le prochain cycle pour que la machine soit busy
        }

        // Attend que la machine ait terminé l'état
        if (!MSM_is_busy(m))
        {
            printf("sequencer: MOTOR step %d/%d done, restoring idle\n",
                   seq->current_step + 1, seq->step_count);

            // Remet la machine en idle (HOLD) dès que l'étape est terminée
            struct SeqIdle* idle = find_idle(seq, m);
            if (idle != 0)
                send_to_machine(idle->machine, idle->state, idle->env);

            seq->current_step++;
            seq->step_sent = 0;
        }
    }
    else if (step->type == SEQ_STEP_ACTION)
    {
        // === ÉTAPE ACTION : Exécution immédiate du callback ===
        printf("sequencer: executing ACTION step %d/%d\n",
               seq->current_step + 1, seq->step_count);

        // Exécute le callback
        if (step->action.callback != 0)
        {
            step->action.callback(step->action.param);
        }

        // Action terminée immédiatement, passe à l'étape suivante
        seq->current_step++;
        seq->step_sent = 0;
    }
    else
    {
        // Type inconnu : erreur
        printf("sequencer: ERROR - unknown step type %d\n", step->type);
        seq->current_step++;
        seq->step_sent = 0;
    }
}


uint8_t sequencer_is_active(const struct Sequencer* seq)
{
    return seq->active;
}


// ============================================================================
// Helpers pour actions courantes
// ============================================================================

// Callback interne pour action GPIO
static void seq_gpio_callback(void* param)
{
    struct SeqGpioAction* gpio = (struct SeqGpioAction*)param;
    if (gpio != 0 && gpio->port != 0)
    {
        HAL_GPIO_WritePin(gpio->port, gpio->pin, gpio->state);
        printf("[SEQ_GPIO] Set pin %d to %d\n", gpio->pin, gpio->state);
    }
}


// Callback interne pour action délai
static void seq_delay_callback(void* param)
{
    struct SeqDelayAction* delay = (struct SeqDelayAction*)param;
    if (delay != 0)
    {
        // Premier appel : mémorise le tick de départ
        if (delay->start_tick == 0)
        {
            delay->start_tick = HAL_GetTick();
            printf("[SEQ_DELAY] Start delay %lu ms\n", delay->delay_ms);
        }

        // Attente active (bloquante)
        uint32_t elapsed = HAL_GetTick() - delay->start_tick;
        if (elapsed < delay->delay_ms)
        {
            HAL_Delay(delay->delay_ms - elapsed);
        }

        printf("[SEQ_DELAY] Delay %lu ms completed\n", delay->delay_ms);
        delay->start_tick = 0;  // Reset pour prochaine utilisation
    }
}


int sequencer_add_gpio(struct Sequencer* seq,
                       GPIO_TypeDef* port,
                       uint16_t pin,
                       GPIO_PinState state)
{
    if (gpio_buffer_index >= SEQ_ACTION_BUFFER_SIZE)
    {
        printf("sequencer: GPIO buffer full\n");
        return -1;
    }

    // Copie les paramètres dans le buffer statique
    gpio_action_buffer[gpio_buffer_index].port  = port;
    gpio_action_buffer[gpio_buffer_index].pin   = pin;
    gpio_action_buffer[gpio_buffer_index].state = state;

    // Ajoute l'action en pointant vers le buffer
    int result = sequencer_add_action(seq, seq_gpio_callback,
                                      &gpio_action_buffer[gpio_buffer_index]);

    if (result == 0)
    {
        gpio_buffer_index++;
    }

    return result;
}


int sequencer_add_delay(struct Sequencer* seq, uint32_t delay_ms)
{
    if (delay_buffer_index >= SEQ_ACTION_BUFFER_SIZE)
    {
        printf("sequencer: Delay buffer full\n");
        return -1;
    }

    // Copie les paramètres dans le buffer statique
    delay_action_buffer[delay_buffer_index].delay_ms   = delay_ms;
    delay_action_buffer[delay_buffer_index].start_tick = 0;

    // Ajoute l'action en pointant vers le buffer
    int result = sequencer_add_action(seq, seq_delay_callback,
                                      &delay_action_buffer[delay_buffer_index]);

    if (result == 0)
    {
        delay_buffer_index++;
    }

    return result;
}
