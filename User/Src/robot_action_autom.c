/*
*
*
**/

#include "robot_action_autom.h"
#include "ax_controller.h"
#include "gpio.h"
#include "i2c_srevo_moteur.h"

//// POSITION AX DEVANT
#define SERAGE_ax_7	 497
#define SERAGE_ax_6	 521

#define CLOSE_ax_7  190
#define CLOSE_ax_6  820

#define OUVERT_ax_7  510
#define OUVERT_ax_6  510

///// POSIION SERVO_MOTEUR  
// on considère que l'on regarde le sens de rotation de face par rapport au cerveau moteur
#define POSITION_DROITE 2400
#define POSITION_MILIEU 1500
#define POSITION_GAUCHE 500



void ax_pince_grap(void* param)
{
    (void)param;  // Paramètre non utilisé
    ax_write_position(7, SERAGE_ax_7);
    ax_write_position(6, SERAGE_ax_6);
}

void ax_pince_close(void* param)
{
    (void)param;  // Paramètre non utilisé
    ax_write_position(6, CLOSE_ax_6);
    //HAL_Delay(3);
    ax_write_position(7, CLOSE_ax_7);
}

void ax_pince_open(void* param)
{
    (void)param;  // Paramètre non utilisé
    ax_write_position(7, OUVERT_ax_7);
    //HAL_Delay(3);
    ax_write_position(6, OUVERT_ax_6);

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
//// ACTIONS INDIVIDUELLES PAR SERVO (pour éviter collisions bus)
////////////////////

// Servo 6 : Positions GRAP
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

// Servo 7 : Positions GRAP
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
//// CAPTEUR COULEUR & PRESENCE TOBOGAN 
////////////////////



////////////////////
////CAPTEUR COULEUR & PRESENCE TOBOGAN 
//// I2C servo moteur 
////////////////////

void trie_tobogan(void* param)
{
    (void)param;
    // couleur bleu = 1 , jaune = 0
    
    uint8_t couleur_element_jeux_2 = HAL_GPIO_ReadPin(cap_couleur_2_GPIO_Port, cap_couleur_2_Pin) ; 

    //variable temporaire car la couleur de equipe pas encore implementer
    // equipe bleu = 1 , jaune = 0
    uint8_t couleur_equipe = 1; 

    /* lorsqu'il n'y a pas d'élement de jeux = 1 et lorsqu'il y a un element de jeux = 0*/
    if (! HAL_GPIO_ReadPin(cap_presence_2_GPIO_Port, cap_presence_2_Pin))
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
    //i2c_servo (1, POSITION_DROITE);
    //i2c_servo (0, POSITION_DROITE);
}

void reset_tobogan(void* param)
{
    (void)param;
    i2c_servo (1, POSITION_MILIEU);
    i2c_servo (0, POSITION_MILIEU);
}