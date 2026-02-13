# ifndef __COORDONNE_ABSOLUE_H
# define __COORDONNE_ABSOLUE_H


/**
 * @file coordonne_absolue.h
 * @brief Contrôle du robot en coordonnées absolues du terrain.
 */


/* ── Types ────────────────────────────────────────────────────────────────── */

/**
 * @brief Coordonnées d'un point sur le terrain (mm).
 */
typedef struct
{
    float x;
    float y;
} Point2D;


/**
 * @brief Pose complète du robot sur le terrain.
 *        x, y en mm ; theta en radians.
 */
typedef struct
{
    float x;
    float y;
    float theta;
} Pose;


/**
 * @brief Sens de déplacement du robot.
 */
typedef enum
{
    FACE_AVANT = 0,
    FACE_ARRIERE = 1,
} Face;


/* ── État global ──────────────────────────────────────────────────────────── */

/**
 * @brief Pose courante du robot. Mise à jour par robot_update_pose().
 */
extern Pose robot_pose;


/* ── Calculs purs ─────────────────────────────────────────────────────────── */

/**
 * @brief Calcule l'angle absolu (radians) que le robot doit avoir pour faire
 *        face au point cible.
 */
float compute_target_angle(Pose from, Point2D target);

/**
 * @brief Calcule la rotation relative (radians) à effectuer pour passer de
 *        l'orientation courante à l'angle cible.
 *        Tient compte du sens de déplacement.
 */
float compute_relative_rotation(float current_theta, float target_angle, Face face);

/**
 * @brief Calcule la distance (mm) à parcourir jusqu'au point cible.
 *        Retourne une valeur négative si face == FACE_ARRIERE.
 */
float compute_distance(Pose from, Point2D target, Face face);



/* ── Mise à jour de pose ──────────────────────────────────────────────────── */

/**
 * @brief Met à jour la pose courante du robot.
 *        Appelée par les fonctions de rotation et de translation.
 */
void robot_update_pose(float new_x, float new_y, float new_theta);

/**
 * @brief Ajoute la longueur donnée aux coordonnées xy absolues, on utilisant l'angle actuel comme sens de déplacement.
 * @param distance the distance added, in mm
 * 
 * @return A pose with updated position
 */
void robot_add_distance(float distance);

/**
 * @brief Ajoute l'angle donné à la rotation absolue, en gardant xy tels quel.
 * @param angle the angle added, in radians
 * 
 * @return A pose with updated position
 */
void robot_add_rotation(float angle);

void robot_update_x(float x) ;
void robot_update_y(float y) ;
void robot_update_theta(float t);
/* ── Commande principale ──────────────────────────────────────────────────── */

/**
 * @brief Enfile dans MV_STATEMACHINE la rotation puis la translation nécessaires
 *        pour amener le robot au point cible.
 *
 * @param target     Coordonnées cibles (mm).
 * @param speed      Vitesse de déplacement (mm/s).
 * @param face       Sens de déplacement.
 * @return 0 si OK, -1 si la statemachine est occupée ou la file pleine.
 */
int goto_xy(Point2D target, float linear_speed, float linear_accel, float angular_speed, float angular_accel, Face face);


int robot_calage(enum Facing facing, float speed, float accel);


/* -- debugging functions -- */

void robot_print_pose();


# endif // __COORDONNE_ABSOLUE_H
