/**
 ******************************************************************************
 * @file    servo_v2.h
 * @brief   Interface simplifiée pour contrôle de servomoteurs via PCA9685
 * @version 2.0
 ******************************************************************************
 * Ce fichier fournit une API minimaliste pour commander des servomoteurs
 * connectés à un module PCA9685 via I2C.
 *
 * Architecture modulaire pour intégration dans projet complexe.
 ******************************************************************************
 */

#ifndef I2C_SERVO_MOTETEUR_H
#define I2C_SERVO_MOTETEUR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Exported constants --------------------------------------------------------*/
#define PCA9685_ADDR_I2C     (0x40 << 1)    // Adresse I2C du PCA9685 (8-bit pour HAL)
#define PCA9685_OSC_FREQ     25000000.0f    // Fréquence oscillateur (25 MHz)
#define PCA9685_PWM_FREQ     50.0f          // Fréquence PWM servo (50 Hz)

#define PCA9685_CANAL_MIN    0              // Canal minimum
#define PCA9685_CANAL_MAX    15             // Canal maximum

/* Positions typiques des servos (en microsecondes) */
#define SERVO_POS_0_DEG      500            // Position 0° (500µs)
#define SERVO_POS_90_DEG     1500           // Position 90° (1500µs)
#define SERVO_POS_180_DEG    2450           // Position 180° (2500µs) ( à 2500 le servo moteur peu beuguer)

/* Exported functions prototypes ---------------------------------------------*/

/**
 * @brief  Initialise le module PCA9685 pour contrôle de servomoteurs
 * @param  hi2c: Pointeur vers le handle I2C à utiliser
 * @note   À appeler une seule fois au démarrage
 * @retval None
 */
void servo_init(I2C_HandleTypeDef *hi2c);

/**
 * @brief  Commande un servomoteur via le PCA9685
 * @param  canal: Numéro du canal (0-15)
 * @param  position_us: Position en microsecondes (500-2500µs typique)
 * @note   Exemple: i2c_servo(0, 1500) → canal 0 à 90°
 * @retval None
 */
void i2c_servo(uint8_t canal, uint16_t position_us);

#ifdef __cplusplus
}
#endif

#endif /* I2C_SERVO_MOTETEUR_H */
