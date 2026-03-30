/*
*
*
**/

#include "robot_action_autom.h"
#include "ax_controller.h"
#include "gpio.h"
#include "i2c_servo_moteur.h"

//// POSITION AX PINCE
#define SERAGE_ax_7	 497
#define SERAGE_ax_6	 521

#define CLOSE_ax_7  190
#define CLOSE_ax_6  820

#define OUVERT_ax_7  550 // le milieu c'est 510
#define OUVERT_ax_6  470

// POSITION AX CACA 
#define POS_EJECTER_AX_CACA 0
#define POS_MILIEU_AX_CACA 400
#define POS_RENTRER_AX_CACA 810

// POSITION AX BANDEROLE
#define POS_OUVERTURE_CURSOR 0
#define POS_FERMETURE_CURSOR 0

///// POSITION SERVO_MOTEUR  
// on considère que l'on regarde le sens de rotation de face par rapport au cerveau moteur
#define POSITION_DROITE 2400
#define POSITION_MILIEU 1500
#define POSITION_GAUCHE 500

#define PORTE_FERMER SERVO_POS_0_DEG
#define PORTE_OUVERTE SERVO_POS_180_DEG

uint8_t couleur_element_jeux_1 ;
uint8_t couleur_element_jeux_2 ;

uint8_t presence_element_jeux_1 ;
uint8_t presence_element_jeux_2 ;

//////////
/// AX CACA & SERVO I2C PORTE 
//////// ejecter element de jeu 

void servo_porte_fermer(void* param)
{
    (void)param; 
    i2c_servo(2,PORTE_FERMER);
}

void servo_porte_ouvert(void* param)
{
    (void)param; 
    i2c_servo(2,PORTE_OUVERTE);
}

void ax_caca_ejecter (void* param)
{
    (void)param; 
    ax_write_position(2, POS_EJECTER_AX_CACA);
}

void ax_caca_milieu (void* param)
{
    (void)param; 
    ax_write_position(2, POS_MILIEU_AX_CACA);
}

void ax_caca_rentrer (void* param)
{
    (void)param; 
    ax_write_position(2, POS_RENTRER_AX_CACA);
}

//////////
/// AX CURSOR
////////

void ax_ouverture_pour_cursor (void* param)
{
    (void)param; 
    ax_write_position(10, POS_OUVERTURE_CURSOR);
}

void ax_fermeture_pour_cursor (void* param)
{
    (void)param; 
    ax_write_position(10, POS_FERMETURE_CURSOR);
}


////////////////////
//// TURN ON AND OFF PUMP
////////////////////

void turn_on_pump_4(void* param)
{
    (void)param;  // Paramètre non utilisé
    HAL_GPIO_WritePin(Pomp_4_GPIO_Port, Pomp_4_Pin, GPIO_PIN_SET);
}

void turn_off_pump_4(void* param)
{
    (void)param;  // Paramètre non utilisé
    HAL_GPIO_WritePin(Pomp_4_GPIO_Port, Pomp_4_Pin, GPIO_PIN_RESET);
}

void turn_on_pump_3(void* param)
{
    (void)param;  // Paramètre non utilisé
    HAL_GPIO_WritePin(Pomp_3_GPIO_Port, Pomp_3_Pin, GPIO_PIN_SET);
}

void turn_off_pump_3(void* param)
{
    (void)param;  // Paramètre non utilisé
    HAL_GPIO_WritePin(Pomp_3_GPIO_Port, Pomp_3_Pin, GPIO_PIN_RESET);
}


////////////////////
//// AX PINCE 
////////////////////

// AX 6 
void ax_servo_6_grap(void* param)
{
    (void)param;
    ax_write_position(6, SERAGE_ax_6);
}

void ax_servo_6_close(void* param)
{
    (void)param;
    ax_write_position(6, CLOSE_ax_6);
}

void ax_servo_6_open(void* param)
{
    (void)param;
    ax_write_position(6, OUVERT_ax_6);
}

// AX 7 :
void ax_servo_7_grap(void* param)
{
    (void)param;
    ax_write_position(7, SERAGE_ax_7);
}

void ax_servo_7_close(void* param)
{
    (void)param;
    ax_write_position(7, CLOSE_ax_7);
}

void ax_servo_7_open(void* param)
{
    (void)param;
    ax_write_position(7, OUVERT_ax_7);
}

////////////////////
////CAPTEUR COULEUR & PRESENCE TOBOGAN 
//// I2C servo moteur 
////////////////////

void scan_tobogan(void* param)
{
    (void)param;
    // couleur bleu = 1 , jaune = 0

    couleur_element_jeux_1 = HAL_GPIO_ReadPin(cap_couleur_1_GPIO_Port, cap_couleur_1_Pin) ; 
    couleur_element_jeux_2 = HAL_GPIO_ReadPin(cap_couleur_2_GPIO_Port, cap_couleur_2_Pin) ; 

    presence_element_jeux_1 = (! HAL_GPIO_ReadPin(cap_presence_1_GPIO_Port, cap_presence_1_Pin)); /* lorsqu'il n'y a pas d'élement de jeux = 1 et lorsqu'il y a un element de jeux = 0 , c''est pour cela que l'on fait le "!" au debut"*/
    presence_element_jeux_2 = (! HAL_GPIO_ReadPin(cap_presence_2_GPIO_Port, cap_presence_2_Pin));
}

void trie_tobogan(void* param)
{
    (void)param;
    //variable temporaire car la couleur de equipe pas encore implementer
    // equipe bleu = 1 , jaune = 0
    uint8_t couleur_equipe = 1; 

    /* lorsqu'il n'y a pas d'élement de jeux = 1 et lorsqu'il y a un element de jeux = 0*/
    if (presence_element_jeux_1)
    {
        if (couleur_element_jeux_1 == couleur_equipe) 
        {
            /* tourner le servo moteur vers la droite ne 
            change pas la couleur de l'element de jeux */
            i2c_servo (1, POSITION_DROITE); // canal 1 c'est le servo moteur 1
        }
        else
        {
            /* on change de couleur l'element de jeux*/
            i2c_servo (1, POSITION_GAUCHE); // canal 0 c'est le servo moteur 1
        }

    }

    /* lorsqu'il n'y a pas d'élement de jeux = 1 et lorsqu'il y a un element de jeux = 0*/
    if (presence_element_jeux_2)
    {
        if (couleur_element_jeux_2 == couleur_equipe) 
        {
            /* tourner le servo moteur vers la droite ne 
            change pas la couleur de l'element de jeux */
            i2c_servo (0, POSITION_DROITE); // canal 0 c'est le servo moteur 2 
        }
        else
        {
            /* on change de couleur l'element de jeux*/
            i2c_servo (0, POSITION_GAUCHE); // canal 0 c'est le servo moteur 2 
        }
    }

    couleur_element_jeux_1 = 0 ;
    couleur_element_jeux_2 = 0 ;
    presence_element_jeux_1 = 0;
    presence_element_jeux_2 = 0;

}

void reset_tobogan(void* param)
{
    (void)param;
    i2c_servo (1, POSITION_MILIEU);
    i2c_servo (0, POSITION_MILIEU);
}

