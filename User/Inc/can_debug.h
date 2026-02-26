/*
 * can_debug.h
 *
 * CAN debug utilities for monitoring CAN frames and CANopen stack state via UART
 */

#ifndef CAN_DEBUG_H
#define CAN_DEBUG_H

#include "main.h"
#include "fdcan.h"
#include "CO_app_STM32.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Print a raw FDCAN frame to UART (ID, DLC, data bytes) */
void can_debug_print_raw_frame(FDCAN_RxHeaderTypeDef* rxHeader, uint8_t* data);

/* Print CANopen node status information */
void can_debug_print_canopen_state(CANopenNodeSTM32* node);

/* Print detailed CANopen stack info (NMT state, errors, SDO activity) */
void can_debug_print_canopen_detailed(CANopenNodeSTM32* node);

/* Enable/disable automatic logging of all received CAN frames */
void can_debug_enable_rx_logging(bool enable);

/* Get current RX logging state */
bool can_debug_is_rx_logging_enabled(void);

/* Hook function to be called from FDCAN RX interrupt/callback */
void can_debug_rx_hook(FDCAN_RxHeaderTypeDef* rxHeader, uint8_t* data);

/* Monitor RPDO variables and print only when they change (call in while loop) */
void can_debug_monitor_rpdo_changes(void);

/* Helper: Print CAN ID in hex */
void can_debug_print_can_id(uint32_t id);

/* Helper: Print data bytes in hex */
void can_debug_print_data_hex(uint8_t* data, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /* CAN_DEBUG_H */
