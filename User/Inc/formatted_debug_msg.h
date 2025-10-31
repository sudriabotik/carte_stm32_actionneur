/**
 * @file formatted_debug_msg.h
 * @author sudriabotik
 * @brief macros to print cleanly formatted debug messages
 */

# ifndef __FORMATTED_DEBUG_MSG_H
# define __FORMATTED_DEBUG_MSG_H

# include "stm32g431xx.h"

// take strings as input
# define DEBUG_MSG_ERROR(src, msg) printf("["src"] [ERROR] : "msg);
# define DEBUG_MSG_ERROR_VAL(src, msg, val) printf("["src"] [ERROR] : "msg, val);

# endif //__FORMATTED_DEBUG_MSG_H