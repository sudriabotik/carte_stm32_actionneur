/*
*
*
**/
#include "robot_action_autom.h"

uint8_t couleur_equipe = 0; //BLEU = 0 , JAUNE = 1 , donc par default c'est bleu

uint8_t couleur_element_jeux_1 ;
uint8_t couleur_element_jeux_2 ;

uint8_t presence_element_jeux_1 ;
uint8_t presence_element_jeux_2 ;

bool element_in_ventouse [4] = {0,0,0,0}; // --> true or false

//uint8_t presence_element_jeux_in_ventouse [4] = {0,0,0,0} // --> true or false

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
    // deplacement de l'ax depuis pos_milieu ou pos_rentrer
    ax_write_position(2, POS_EJECTER_AX_CACA);
    ax_caca_situation = POS_EJECTER_AX_CACA;
    bottom_place[TOB_INT] = 2; // ax bloque
    bottom_place[TOB_EXT] = 2; // ax bloque
}

void ax_caca_milieu (void* param)
{
    (void)param; 
    // deplacement de l'ax depuis pos_rentrer
    ax_write_position(2, POS_MILIEU_AX_CACA);
    ax_caca_situation = POS_MILIEU_AX_CACA;
    bottom_place[TOB_INT] = 2; // ax bloque
    printf("[DBG] caca_mil ax=%u I=%u E=%u\n",
        ax_caca_situation, bottom_place[TOB_INT], bottom_place[TOB_EXT]);

}

void ax_caca_milieu_reculer(void* param)
{
    (void)param; 
    // deplacement de l'ax depuis pos_ejecter vers pos_milieu
    ax_write_position(2, POS_MILIEU_AX_CACA);
    ax_caca_situation = POS_MILIEU_AX_CACA;
    bottom_place[TOB_EXT] = 2; // ax bloque
    bottom_place[TOB_INT] = 2; // ax bloque
    elements_jeux_tombe();
}

void ax_caca_rentrer (void* param)
{
    (void)param; 
    ax_write_position(2, POS_RENTRER_AX_CACA);
    ax_caca_situation = POS_RENTRER_AX_CACA;
}

void ax_caca_calage(void* param)
{
    (void)param;
    if (ax_caca_situation == POS_EJECTER_AX_CACA)
    {
        ax_write_position(2, POS_CALAGE_AX_CACA);
    }
}

//////////
/// AX CURSOR
////////

void ax_ouverture_cursor (void* param)
{
    (void)param; 
    ax_write_position(1, POS_OUVERTURE_CURSOR);
}

void ax_fermeture_cursor (void* param)
{
    (void)param; 
    ax_write_position(1, POS_FERMETURE_CURSOR);
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
    element_in_ventouse[0] = 0;
    element_in_ventouse[2] = 0;
    printf("[DBG] pump_4 OFF → v0=0 v2=0\n");
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
    element_in_ventouse[3] = 0;
    element_in_ventouse[1] = 0;
    printf("[DBG] pump_3 OFF → v1=0 v3=0\n");
}


////////////////////
//// AX PINCE 
////////////////////

// AX 6 
void ax_servo_6_grap(void* param)
{
    (void)param;
    //ax_write_position(6, SERAGE_ax_6);
    ax_write_position_and_speed(6, SERAGE_ax_6, 80);
}

void ax_servo_6_close(void* param)
{
    (void)param;
    //ax_write_position(6, CLOSE_ax_6);
    ax_write_position_and_speed(6, CLOSE_ax_6, 250);
}

void ax_servo_6_open(void* param)
{
    (void)param;
    //ax_write_position(6, OUVERT_ax_6);
    ax_write_position_and_speed(6, OUVERT_ax_6, 500);
}

// AX 7 :
void ax_servo_7_grap(void* param)
{
    (void)param;
    //ax_write_position(7, SERAGE_ax_7);
    ax_write_position_and_speed(7, SERAGE_ax_7, 80);
}

void ax_servo_7_close(void* param)
{
    (void)param;
    //ax_write_position(7, CLOSE_ax_7);
    ax_write_position_and_speed(7, CLOSE_ax_7, 250);
}

void ax_servo_7_open(void* param)
{
    (void)param;
    //ax_write_position(7, OUVERT_ax_7);
    ax_write_position_and_speed(7, OUVERT_ax_7, 500);
}

////////////////////
////CAPTEUR COULEUR & PRESENCE TOBOGAN 
//// I2C servo moteur 
////////////////////

// element_jeux_1 -> dans le toboban intérieur

void scan_tobogan(void* param)
{
    uint8_t position = (uint8_t)(uintptr_t)param;

    bool pos_depos_3_1 ;
    bool pos_depos_2_0 ;

    if (position == 1 )
    {
        pos_depos_3_1 = true;
        pos_depos_2_0 = false;
    }
    else
    {
        pos_depos_3_1 = false;
        pos_depos_2_0 = true;
    }

    // couleur des elements de jeux bleu = 1 , jaune = 0
    couleur_element_jeux_1 = HAL_GPIO_ReadPin(cap_couleur_1_GPIO_Port, cap_couleur_1_Pin) ;
    couleur_element_jeux_2 = HAL_GPIO_ReadPin(cap_couleur_2_GPIO_Port, cap_couleur_2_Pin) ;

    // lorsqu'il n'y a pas d'élement de jeux = 1 et lorsqu'il y a un element de jeux = 0  c''est pour cela que l'on fait le "!" au debut
    presence_element_jeux_1 = (! HAL_GPIO_ReadPin(cap_presence_1_GPIO_Port, cap_presence_1_Pin));
    presence_element_jeux_2 = (! HAL_GPIO_ReadPin(cap_presence_2_GPIO_Port, cap_presence_2_Pin));

    printf("[DBG] scan_tobogan pos=%u p1=%u p2=%u\n", position, presence_element_jeux_1, presence_element_jeux_2);

    //if ( (presence_element_jeux_1 == 1) || (presence_element_jeux_2 ) )
    //{
    //    presence_element_jeux_in_ventouse = true;
    //}
    if (pos_depos_3_1)
    {
        element_in_ventouse[1] = presence_element_jeux_1;
        element_in_ventouse[3] = presence_element_jeux_2;
        printf("[DBG] → v1=%u v3=%u (pos 1)\n", element_in_ventouse[1], element_in_ventouse[3]);
    }

    if(pos_depos_2_0)
    {
        element_in_ventouse[0] = presence_element_jeux_1;
        element_in_ventouse[2] = presence_element_jeux_2;
        printf("[DBG] → v0=%u v2=%u (pos 2)\n", element_in_ventouse[0], element_in_ventouse[2]);
    }

}

///// OLD ////
/* 
void trie_tobogan(void* param)
{
    (void)param;

    // rapelle : equipe bleu = 0 , jaune = 1
    // rapelle : couleur des elements de jeux bleu = 1 , jaune = 0

    // lorsque l'on fait le trie du tobogan les elements de jeux 
    //on deja était lacher des ventouse donc on actualise la variable: 
    if (presence_element_jeux_1)
    {
        add_element_jeux_in_tob_int();
        if (couleur_element_jeux_1 != couleur_equipe) 
        {
            // tourner le servo moteur vers la droite ne 
            //change pas la couleur de l'element de jeux 
            i2c_servo (1, POSITION_DROITE); // canal 1 c'est le servo moteur 1
        }
        else
        {
            // on change de couleur l'element de jeux
            i2c_servo (1, POSITION_GAUCHE); // canal 1 c'est le servo moteur 1
        }

    }

    // lorsqu'il n'y a pas d'élement de jeux = 1 et lorsqu'il y a un element de jeux = 0
    if (presence_element_jeux_2)
    {
        add_element_jeux_in_tob_ext();
        if (couleur_element_jeux_2 != couleur_equipe) 
        {
            // tourner le servo moteur vers la droite ne 
            //change pas la couleur de l'element de jeux 
            i2c_servo (0, POSITION_DROITE); // canal 0 c'est le servo moteur 2 
        }
        else
        {
            // on change de couleur l'element de jeux
            i2c_servo (0, POSITION_GAUCHE); // canal 0 c'est le servo moteur 2 
        }
    }

    couleur_element_jeux_1 = 0 ;
    couleur_element_jeux_2 = 0 ;
    presence_element_jeux_1 = 0;
    presence_element_jeux_2 = 0;
}
*/

void reset_tobogan(void* param)
{
    (void)param;
    i2c_servo (1, POSITION_MILIEU);
    i2c_servo (0, POSITION_MILIEU);
}

int check_in_ventouse(void)
{
    if (element_in_ventouse[0] ||
        element_in_ventouse[1] ||
        element_in_ventouse[2] ||
        element_in_ventouse[3]   ) return 1;

    else return 0;
}

/////////
//* GESTION DU TOBOGAN CODE INTELIGENT*/
////////

/* ces 3 liste représente les emplacements dans les tobogans. 
0 = aucune presence d'élement de jeux
1 = présence d'élements de jeux
2 = ax_caca bloque le chemin

[A,B]
l'emplacement numero A = tobogan intérieure 
l'emplacement numero B = tobogan extérieure  
*/

//acronyme
// tob_int = tobogan_intérieur 
// tob_ext = tobogan_exterieur


///// GESTION TOBOGAN /////
uint8_t top_place [2] = {0,0};
uint8_t middle_place [2] = {0,0};
uint8_t bottom_place [2] = {2,2};

uint16_t ax_caca_situation = POS_EJECTER_AX_CACA ; // par default l'ax caca est en position ejecter

// situation 0 --> POS_RENTRER_AX_CACA // cette ligne je crois inutile
// situation 1 --> POS_MILIEU_AX_CACA // cette ligne je crois inutile
// situation 2 --> POS_EJECTER_AX_CACA // cette ligne je crois inutile

// position par default l'axe qui fait caca est en position POS_EJECTER_AX_CACA

int check_element_jeux_tob_ext()
{
    if (top_place[TOB_EXT] == 1) return -1 ; // return -1 -> tobogan_int deja plein
    else return 0; // return 0 -> ok 
}

int check_element_jeux_tob_int()
{
    if (top_place[TOB_INT] == 1) return -1 ; // return -1 -> tobogan_int deja plein
    else return 0; // return 0 -> ok 
}

void add_element_jeux_in_tob_int ()
{
    if (check_element_jeux_tob_int() == -1) return;

    if (bottom_place[TOB_INT]== 0) bottom_place[0] = 1;

    else if (middle_place[TOB_INT]==0) middle_place[0] = 1;

    else if (top_place[TOB_INT]==0) top_place[0] = 1;
}

void add_element_jeux_in_tob_ext ()
{
    if (check_element_jeux_tob_ext() == -1) return;

    if (bottom_place[TOB_EXT]== 0) bottom_place[1] = 1;

    else if (middle_place[TOB_EXT]==0) middle_place[1] = 1;

    else if (top_place[TOB_EXT]==0) top_place[1] = 1;
}


void elements_jeux_tombe()
{
    /*
    Lorsque le ax caca bouge il expulse ou fait tomber les elements
    de jeux plus bas dans le tobogan donc il faut update la position
    des elements de jeux dans le tobogan.

    Télément de jeu qui était Pour la liste top place passe dans la liste Middle place Est l élément de jeu qui était dans la liste Middle place passe dans la liste botom place
    et tous cela en respectant les 2 tobogan different on peu voir ça comme des tuyaux séparer qui dépose au meme endroit à la fin ( au botom).

    */
     // Tobogan intérieur 
    if ((middle_place[TOB_INT] ==1) && (bottom_place[TOB_INT]==0))
    {
        bottom_place[TOB_INT] = middle_place[TOB_INT];
        middle_place[TOB_INT] = 0;
    }
    if ((top_place[TOB_INT] == 1) &&  (middle_place[TOB_INT]==0)) {
        middle_place[TOB_INT] = top_place[TOB_INT];
        top_place[TOB_INT] = 0;
    }
    
    // Tobogan extérieur 
   if ((middle_place[TOB_EXT] ==1) && (bottom_place[TOB_EXT]==0))
    {
        bottom_place[TOB_EXT] = middle_place[TOB_EXT];
        middle_place[TOB_EXT] = 0;
    }
    if ((top_place[TOB_EXT] == 1) &&  (middle_place[TOB_EXT]==0)) {
        middle_place[TOB_EXT] = top_place[TOB_EXT];
        top_place[TOB_EXT] = 0;
    }
}


void fermer_porte_et_rentrer_ax(void* param)
{
    (void)param;  // Paramètre non utilisé
    // ici on pourra peut-être ameliorer le truc si besoin
    // en disant de reculer uniquement vers position milieu si middle_place[TOB_INT]==0
    // mais ça implique d'en avoir consience par la suite
    servo_porte_fermer(NULL);
    if (ax_caca_situation == POS_EJECTER_AX_CACA)
    {
        ax_caca_rentrer(NULL);
        ax_caca_situation = POS_RENTRER_AX_CACA;
        bottom_place[TOB_INT] = 0; // la place est libre
        bottom_place[TOB_EXT] = 0; // la place est libre
    }
    else if (ax_caca_situation == POS_MILIEU_AX_CACA)
    {
        ax_caca_rentrer(NULL);
        ax_caca_situation = POS_RENTRER_AX_CACA;
        bottom_place[TOB_INT] = 0; // la place est libre
    }
    elements_jeux_tombe();
}

void print_state_tobotan()
{
    printf("[state tobogan]  E_b=%u I_b=%u E_m=%u I_m=%u E_t=%u I_t=%u \n",
        bottom_place[TOB_EXT], bottom_place[TOB_INT], middle_place[TOB_EXT], middle_place[TOB_INT], top_place[TOB_EXT],top_place[TOB_INT] );
}

// actuellement non utiliser : 
/* 
void ejecter_element_jeux(uint8_t num_element_jeux_a_ejecter,int vitesse_deplacement_robot)
{
    if(ax_caca_situation == POS_EJECTER_AX_CACA )
    {   
        servo_porte_fermer(NULL);
        ax_caca_rentrer(NULL);
        ax_caca_situation = POS_RENTRER_AX_CACA;
        bottom_place[TOB_INT] = 0; // la place est libre
        bottom_place[TOB_EXT] = 0; // la place est libre
        elements_jeux_tombe();
    }  

    // il faut attendre un peu le mouvement de lax mais on ne peut pas utiliser la fonction delay il faut utiliser une fonction 
    // qui prend en parametre le temps entre chaque interruption pour calculer le temps. 

    if((bottom_place[TOB_INT] == 1) && (bottom_place[TOB_EXT] == 1) && (ax_caca_situation == POS_RENTRER_AX_CACA) )
    {
        if (num_element_jeux_a_ejecter == 1)
        {
            servo_porte_ouvert(NULL);
            ax_caca_milieu(NULL);
            ax_caca_situation = POS_MILIEU_AX_CACA;
            bottom_place[TOB_INT] = 2; // ax bloque

            // est ce que l on ferme le servo moteur ?
        }
        else if(num_element_jeux_a_ejecter == 2) // ce ca de figure c est pour ejecter en roulant les elements de jeux.
        {
            servo_porte_ouvert(NULL);
            ax_caca_milieu(NULL);
            ax_caca_situation = POS_MILIEU_AX_CACA;
            bottom_place[TOB_INT] = 2; // ax qui prend la place
            // plus la vitesse du robot est rapide moins on attend de temps
            //pour deposer les elements de jeux. 
            //
            int attente_dynamique = (int)((100.0f / vitesse_deplacement_robot) * 5.0f); 
            // delay_inteligent(attente_dynamique)
            ax_caca_ejecter(NULL);
            ax_caca_situation = POS_EJECTER_AX_CACA;
            bottom_place[TOB_EXT] = 2; // ax bloque
        }
    }
    if(bottom_place[TOB_EXT] == 1)
    {
        if (num_element_jeux_a_ejecter==1)
        {
            servo_porte_ouvert(NULL);
            ax_caca_ejecter(NULL);
            ax_caca_situation = POS_EJECTER_AX_CACA;
            bottom_place[TOB_EXT] = 2; // ax bloque
        }
    }
}

*/ 


