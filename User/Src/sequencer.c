#include "sequencer.h"

#include <string.h>
#include <stdio.h>


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
                         void* param,
                         uint32_t delay_after_ms)
{
    if (seq->step_count >= SEQUENCER_MAX_STEPS)
    {
        printf("sequencer: full, cannot add action\n");
        return -1;
    }

    // Type ACTION
    seq->steps[seq->step_count].type = SEQ_STEP_ACTION;
    seq->steps[seq->step_count].action.callback       = callback;
    seq->steps[seq->step_count].action.param          = param;
    seq->steps[seq->step_count].action.delay_after_ms = delay_after_ms;
    seq->steps[seq->step_count].action.elapsed_time_ms = 0;
    seq->steps[seq->step_count].action.executed       = 0;

    printf("sequencer: add ACTION step %d (delay=%lums)\n", seq->step_count, delay_after_ms);
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
}


void sequencer_update(struct Sequencer* seq, uint32_t delta_time_ms)
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
        // === ÉTAPE ACTION : Exécution + délai optionnel ===

        // 1. Exécuter le callback (une seule fois)
        if (!step->action.executed)
        {
            printf("sequencer: executing ACTION step %d/%d\n",
                   seq->current_step + 1, seq->step_count);

            if (step->action.callback != 0)
            {
                step->action.callback(step->action.param);
            }

            step->action.executed = 1;

            // Si pas de délai, passer immédiatement à l'étape suivante
            if (step->action.delay_after_ms == 0)
            {
                seq->current_step++;
                seq->step_sent = 0;
                return;
            }
        }

        // 2. Attendre le délai (si delay_after_ms > 0)
        step->action.elapsed_time_ms += delta_time_ms;

        if (step->action.elapsed_time_ms >= step->action.delay_after_ms)
        {
            printf("sequencer: ACTION step %d/%d delay completed (%lums)\n",
                   seq->current_step + 1, seq->step_count, step->action.delay_after_ms);

            seq->current_step++;
            seq->step_sent = 0;
        }
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
    // 1 = séquence en cours, 0 = terminée ou pas encore démarrée
    return seq->active;
}
