#ifndef __ROB_SEQ_DEPOSE_H
#define __ROB_SEQ_DEPOSE_H

/**
 * @file rob_seq_depose.h
 *
 * @brief Séquences de dépôt et triage pour le toboggan
 *
 * Ce fichier contient les séquences complexes liées au dépôt d'éléments
 * dans le toboggan, incluant le triage par couleur avec les servos.
 *
 * Utilise le système de sous-séquences pour exécuter les actions servo
 * immédiatement lors du dépôt.
 */

#include "sequencer.h"


/**
 * @brief Séquence de triage du toboggan avec servos (version sous-séquence)
 *
 * Cette fonction est appelée comme action callback et crée une sous-séquence
 * qui trie les éléments selon leur couleur en contrôlant les servos du toboggan.
 *
 * Le séquenceur parent se met en PAUSE pendant l'exécution des servos,
 * puis reprend automatiquement une fois le triage terminé.
 *
 * @param param Pointeur vers le séquenceur parent (struct Sequencer*)
 *
 * Comportement :
 * - Vérifie la présence et couleur des éléments (presence_element_jeux_1/2, couleur_element_jeux_1/2)
 * - Commande les servos 0 et 1 pour orienter les éléments
 * - Délais non-bloquants gérés par le séquenceur
 * - Nettoie les variables à la fin
 *
 * Exemple d'utilisation :
 *   sequencer_add_action(&main_seq, trie_tobogan_v2, &main_seq, 0);
 */
void trie_tobogan_v2(void* param);


/**
 * @brief Séquence complète de dépôt d'éléments dans le toboggan
 *
 * Orchestre le dépôt complet avec :
 * - Ouverture/fermeture pince
 * - Déplacements ascenseurs H/V
 * - Scan toboggan
 * - Triage avec sous-séquences servo
 * - Gestion intelligente selon l'état du toboggan (vide/plein/partiellement rempli)
 *
 * @param seq Séquenceur à remplir avec la séquence de dépôt
 */
void enchement_seq_depose(struct Sequencer* seq);


#endif // __ROB_SEQ_DEPOSE_H