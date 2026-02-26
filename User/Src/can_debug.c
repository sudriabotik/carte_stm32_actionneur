/*
 * can_debug.c
 *
 * CAN debug utilities for monitoring CAN frames and CANopen stack state via UART
 */

#include "can_debug.h"
#include "CANopen.h"
#include "OD.h"
#include <stdio.h>
#include <inttypes.h>

/* Static variable to control automatic RX logging */
static bool rx_logging_enabled = false;

/* Print a raw FDCAN frame to UART */
void can_debug_print_raw_frame(FDCAN_RxHeaderTypeDef* rxHeader, uint8_t* data) {
    if (rxHeader == NULL || data == NULL) {
        return;
    }

    printf("[CAN_RX] ID=0x%03lX ", rxHeader->Identifier);

    // Print RTR flag
    if (rxHeader->RxFrameType == FDCAN_REMOTE_FRAME) {
        printf("RTR ");
    }

    // Decode DLC
    uint8_t dlc = 0;
    switch (rxHeader->DataLength) {
        case FDCAN_DLC_BYTES_0: dlc = 0; break;
        case FDCAN_DLC_BYTES_1: dlc = 1; break;
        case FDCAN_DLC_BYTES_2: dlc = 2; break;
        case FDCAN_DLC_BYTES_3: dlc = 3; break;
        case FDCAN_DLC_BYTES_4: dlc = 4; break;
        case FDCAN_DLC_BYTES_5: dlc = 5; break;
        case FDCAN_DLC_BYTES_6: dlc = 6; break;
        case FDCAN_DLC_BYTES_7: dlc = 7; break;
        case FDCAN_DLC_BYTES_8: dlc = 8; break;
        default: dlc = 0; break;
    }

    printf("DLC=%u DATA=[ ", dlc);
    for (uint8_t i = 0; i < dlc; i++) {
        printf("%02X ", data[i]);
    }
    printf("]\n");
}

/* Helper: Print CAN ID in hex */
void can_debug_print_can_id(uint32_t id) {
    printf("0x%03lX", id);
}

/* Helper: Print data bytes in hex */
void can_debug_print_data_hex(uint8_t* data, uint8_t len) {
    if (data == NULL) {
        return;
    }
    printf("[ ");
    for (uint8_t i = 0; i < len; i++) {
        printf("%02X ", data[i]);
    }
    printf("]");
}

/* Print CANopen node status information */
void can_debug_print_canopen_state(CANopenNodeSTM32* node) {
    if (node == NULL || node->canOpenStack == NULL) {
        printf("[CANopen] Node not initialized\n");
        return;
    }

    CO_t* co = node->canOpenStack;

    printf("[CANopen] Node ID=%u ", node->activeNodeID);

    // NMT State
    if (co->NMT != NULL) {
        switch (co->NMT->operatingState) {
            case CO_NMT_INITIALIZING:
                printf("State=INITIALIZING ");
                break;
            case CO_NMT_PRE_OPERATIONAL:
                printf("State=PRE_OPERATIONAL ");
                break;
            case CO_NMT_OPERATIONAL:
                printf("State=OPERATIONAL ");
                break;
            case CO_NMT_STOPPED:
                printf("State=STOPPED ");
                break;
            default:
                printf("State=UNKNOWN ");
                break;
        }
    }

    // LED status
    printf("LED_G=%u LED_R=%u\n", node->outStatusLEDGreen, node->outStatusLEDRed);
}

/* Print detailed CANopen stack info */
void can_debug_print_canopen_detailed(CANopenNodeSTM32* node) {
    if (node == NULL || node->canOpenStack == NULL) {
        printf("[CANopen] Node not initialized\n");
        return;
    }

    CO_t* co = node->canOpenStack;

    printf("===== CANopen Detailed Status =====\n");
    printf("Node ID: %u (desired: %u)\n", node->activeNodeID, node->desiredNodeID);
    printf("Baudrate: %u kbps\n", node->baudrate);

    // NMT State
    if (co->NMT != NULL) {
        printf("NMT State: ");
        switch (co->NMT->operatingState) {
            case CO_NMT_INITIALIZING:
                printf("INITIALIZING\n");
                break;
            case CO_NMT_PRE_OPERATIONAL:
                printf("PRE_OPERATIONAL\n");
                break;
            case CO_NMT_OPERATIONAL:
                printf("OPERATIONAL\n");
                break;
            case CO_NMT_STOPPED:
                printf("STOPPED\n");
                break;
            default:
                printf("UNKNOWN (%u)\n", co->NMT->operatingState);
                break;
        }
    }

    // Emergency object
    if (co->em != NULL) {
        // Check if any error status bits are set
        bool has_errors = false;
        for (uint8_t i = 0; i < (CO_CONFIG_EM_ERR_STATUS_BITS_COUNT / 8U); i++) {
            if (co->em->errorStatusBits[i] != 0) {
                has_errors = true;
                break;
            }
        }
        printf("Emergency: %s\n", has_errors ? "ERRORS DETECTED" : "No errors");
    }

    // SDO Server
    if (co->SDOserver != NULL) {
        printf("SDO Server: Configured (index 0)\n");
    }

    // PDO status
    if (co->TPDO != NULL) {
        printf("TPDO: Configured\n");
    }
    if (co->RPDO != NULL) {
        printf("RPDO: Configured\n");
    }

    // SYNC
    if (co->SYNC != NULL) {
        printf("SYNC: Available\n");
    }

    printf("LED Status: Green=%u Red=%u\n", node->outStatusLEDGreen, node->outStatusLEDRed);
    printf("===================================\n");
}

/* Enable/disable automatic logging of all received CAN frames */
void can_debug_enable_rx_logging(bool enable) {
    rx_logging_enabled = enable;
    if (enable) {
        printf("[CAN_DEBUG] RX logging enabled\n");
    } else {
        printf("[CAN_DEBUG] RX logging disabled\n");
    }
}

/* Get current RX logging state */
bool can_debug_is_rx_logging_enabled(void) {
    return rx_logging_enabled;
}

/* Hook function to be called from FDCAN RX interrupt/callback */
void can_debug_rx_hook(FDCAN_RxHeaderTypeDef* rxHeader, uint8_t* data) {
    if (rx_logging_enabled && rxHeader != NULL && data != NULL) {
        can_debug_print_raw_frame(rxHeader, data);
    }
}

/* Monitor RPDO variables and print only when they change */
void can_debug_monitor_rpdo_changes(void) {
    // Variables statiques pour mémoriser les anciennes valeurs
    static uint16_t last_action_id = 0xFFFF;  // Init avec valeur invalide
    static int16_t last_param_1 = 0x7FFF;
    static int16_t last_param_2 = 0x7FFF;
    static int16_t last_param_3 = 0x7FFF;
    static int16_t last_param_4 = 0x7FFF;
    static int16_t last_param_5 = 0x7FFF;
    static int16_t last_param_6 = 0x7FFF;
    static uint16_t last_command_id = 0xFFFF;
    static bool first_call = true;

    // Lecture des valeurs actuelles depuis OD_RAM
    uint16_t action_id = OD_RAM.x2000_ACTION_ID;
    int16_t param_1 = OD_RAM.x2001_param_1;
    int16_t param_2 = OD_RAM.x2002_param_2;
    int16_t param_3 = OD_RAM.x2003_param_3;
    int16_t param_4 = OD_RAM.x2004_param_4;
    int16_t param_5 = OD_RAM.x2005_param_5;
    int16_t param_6 = OD_RAM.x2006_param_6;
    uint16_t command_id = OD_RAM.x2007_command_ID;

    // Détection de changements RPDO1 (COB-ID 0x202)
    if (first_call || action_id != last_action_id || param_1 != last_param_1 ||
        param_2 != last_param_2 || param_3 != last_param_3) {

        printf("[RPDO1 0x202] ACTION_ID=%u param1=%d param2=%d param3=%d\n",
               action_id, param_1, param_2, param_3);

        last_action_id = action_id;
        last_param_1 = param_1;
        last_param_2 = param_2;
        last_param_3 = param_3;
    }

    // Détection de changements RPDO2 (COB-ID 0x302)
    if (first_call || param_4 != last_param_4 || param_5 != last_param_5 ||
        param_6 != last_param_6 || command_id != last_command_id) {

        printf("[RPDO2 0x302] param4=%d param5=%d param6=%d CMD_ID=%u\n",
               param_4, param_5, param_6, command_id);

        last_param_4 = param_4;
        last_param_5 = param_5;
        last_param_6 = param_6;
        last_command_id = command_id;
    }

    first_call = false;
}
