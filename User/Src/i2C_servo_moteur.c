/**
 ******************************************************************************
 * @file    servo_v2.c
 * @brief   Implémentation simplifiée du contrôle servo via PCA9685
 * @version 2.0
 ******************************************************************************
 * Architecture modulaire en 2 sous-parties:
 * 1. Calcul des adresses (PCA + canal)
 * 2. Calcul et préparation des valeurs PWM
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "i2c_servo_moteur.h"

/* Private defines -----------------------------------------------------------*/
#define PCA9685_MODE1        0x00           // Registre MODE1
#define PCA9685_MODE2        0x01           // Registre MODE2
#define PCA9685_PRESCALE     0xFE           // Registre PRESCALE (fréquence)
#define PCA9685_LED0_BASE    0x06           // Registre de base pour LED0

#define MODE1_SLEEP          0x10           // Bit SLEEP dans MODE1
#define MODE1_RESTART        0x80           // Bit RESTART dans MODE1
#define MODE1_AI             0x20           // Bit Auto-Increment dans MODE1
#define MODE2_OUTDRV         0x04           // Bit OUTDRV (push-pull) dans MODE2

/* Private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef *phi2c = NULL;     // Pointeur vers le handle I2C

/* Private function prototypes -----------------------------------------------*/
static uint8_t calcul_adresse_registre(uint8_t canal);
static void calcul_valeurs_pwm(uint16_t position_us, uint8_t data[4]);
static void pca_write_register(uint8_t reg, uint8_t val);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Sous-partie 1: Calcule l'adresse du registre pour un canal donné
 * @param  canal: Numéro du canal (0-15)
 * @retval Adresse du registre LED_ON_L pour ce canal
 * @note   Formule: 0x06 + (canal × 4)
 *         Chaque canal occupe 4 registres consécutifs (ON_L, ON_H, OFF_L, OFF_H)
 */
static uint8_t calcul_adresse_registre(uint8_t canal)
{
    // Limiter le canal à la plage valide
    if (canal > PCA9685_CANAL_MAX) {
        canal = PCA9685_CANAL_MAX;
    }

    // Calcul de l'adresse de base du canal
    // Canal 0 → 0x06, Canal 1 → 0x0A, Canal 2 → 0x0E, etc.
    return PCA9685_LED0_BASE + (canal * 4);
}

/**
 * @brief  Sous-partie 2: Convertit la position en µs et prépare les 4 octets à envoyer
 * @param  position_us: Position en microsecondes (500-2500µs typique)
 * @param  data: Tableau de 4 octets à remplir [ON_L, ON_H, OFF_L, OFF_H]
 * @note   Conversion: µs → ticks 12-bit (0-4095)
 *         Décomposition: ticks → 2 octets (LOW + HIGH)
 */
static void calcul_valeurs_pwm(uint16_t position_us, uint8_t data[4])
{
    // Calcul de la durée d'un tick en microsecondes
    // À 50Hz: période = 20000µs, résolution = 20000/4096 ≈ 4.88µs
    float period_us = 1000000.0f / PCA9685_PWM_FREQ;  // 20000µs
    float tick_us   = period_us / 4096.0f;             // ~4.88µs

    // Conversion microsecondes → ticks
    uint16_t ticks = (uint16_t)(position_us / tick_us + 0.5f);

    // Saturation à 4095 (12 bits max)
    if (ticks > 4095) {
        ticks = 4095;
    }

    // Préparation des 4 octets à envoyer au PCA9685
    // Pour servos: ON toujours à 0 (signal démarre au début de la période)
    data[0] = 0x00;                      // ON_L = 0
    data[1] = 0x00;                      // ON_H = 0
    data[2] = (uint8_t)(ticks & 0xFF);   // OFF_L = 8 bits de poids faible
    data[3] = (uint8_t)(ticks >> 8);     // OFF_H = 4 bits de poids fort
}

/**
 * @brief  Écrit un octet dans un registre du PCA9685
 * @param  reg: Adresse du registre
 * @param  val: Valeur à écrire
 * @retval None
 */
static void pca_write_register(uint8_t reg, uint8_t val)
{
    HAL_I2C_Mem_Write(phi2c, PCA9685_ADDR_I2C, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
}

/* Public functions ----------------------------------------------------------*/

/**
 * @brief  Initialise le module PCA9685 pour contrôle de servomoteurs
 * @param  hi2c: Pointeur vers le handle I2C à utiliser
 * @note   Configure le mode push-pull, auto-increment et fréquence 50Hz
 * @retval None
 */
void servo_init(I2C_HandleTypeDef *hi2c)
{
    // Sauvegarde du handle I2C
    phi2c = hi2c;

    // Délai pour stabilisation après power-on
    HAL_Delay(10);

    // Configuration MODE2: OUTDRV=1 (sorties push-pull)
    pca_write_register(PCA9685_MODE2, MODE2_OUTDRV);

    // Configuration MODE1: AI=1 (auto-increment des registres)
    pca_write_register(PCA9685_MODE1, MODE1_AI);

    // Calcul du prescaler pour 50Hz
    // Formule: prescaler = (OSC_FREQ / (4096 × PWM_FREQ)) - 1
    float prescaleval = (PCA9685_OSC_FREQ / (4096.0f * PCA9685_PWM_FREQ)) - 1.0f;
    uint8_t prescale = (uint8_t)(prescaleval + 0.5f);

    // Lire MODE1 actuel
    uint8_t oldmode;
    HAL_I2C_Mem_Read(phi2c, PCA9685_ADDR_I2C, PCA9685_MODE1,
                     I2C_MEMADD_SIZE_8BIT, &oldmode, 1, 100);

    // Passer en mode SLEEP pour modifier PRESCALE
    uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP;
    pca_write_register(PCA9685_MODE1, newmode);

    // Écrire la nouvelle fréquence
    pca_write_register(PCA9685_PRESCALE, prescale);

    // Sortir du mode SLEEP
    pca_write_register(PCA9685_MODE1, oldmode);
    HAL_Delay(1);

    // Activer RESTART + AI
    pca_write_register(PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI);
}

/**
 * @brief  Commande un servomoteur via le PCA9685
 * @param  canal: Numéro du canal (0-15)
 * @param  position_us: Position en microsecondes (500-2500µs typique)
 * @note   Combine les 2 sous-parties: calcul adresse + calcul valeurs
 * @retval None
 *
 * @example
 *   i2c_servo(0, 1500);  // Canal 0 à 90° (1500µs)
 *   i2c_servo(3, 500);   // Canal 3 à 0° (500µs)
 *   i2c_servo(5, 2500);  // Canal 5 à 180° (2500µs)
 */
void i2c_servo(uint8_t canal, uint16_t position_us)
{
    // SOUS-PARTIE 1: Calcul de l'adresse du registre
    uint8_t reg_adresse = calcul_adresse_registre(canal);

    // SOUS-PARTIE 2: Calcul des valeurs PWM et préparation des données
    uint8_t data[4];  // [ON_L, ON_H, OFF_L, OFF_H]
    calcul_valeurs_pwm(position_us, data);

    // Envoi de la trame I2C: écriture des 4 octets
    // Le PCA9685 avec AI=1 auto-incrémente l'adresse registre
    HAL_I2C_Mem_Write(phi2c, PCA9685_ADDR_I2C, reg_adresse,
                      I2C_MEMADD_SIZE_8BIT, data, 4, 100);
}
