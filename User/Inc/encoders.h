# ifndef __ENCODERS_H
# define __ENCODERS_H

/**
 * 
 * @brief Contains code to handle encoders implemented via STM32's HAL timer encoder mode.
 * 
 */


// hal includes
# include "stm32g431xx.h"

// other includes
# include "tim.h"


enum encoder_situation
{
	ENCODER_SITUATION_NOTHING = 0,
	ENCODER_SITUATION_UNDERFLOW_CLOSE = 1,
	ENCODER_SITUATION_OVERFLOW_CLOSE = 2,
};


/**
 * @brief A struct to handle 16 bits timers and provide a 32 bits total count.
 * 
 * 
 */
struct Encoder16Handle
{
	/** The 32 bits count computed from the timer counter and detected rollovers */
	int32_t total_count;

	/** The delta between now and the last update */
	int32_t total_count_delta;

	/** Increases at each timer rollover and decreases at each rollback */
	int32_t rollover_count;

	/** A pointer to the timer CNT we are watching for this encoder 
	 * It is stored in an uint32_t, probably for compatibility between 16 and 32 bits timers.
	*/
	volatile uint32_t *tim_counter;

	/** Store if the encoder is close to underflow or overflow */
	uint32_t situation;

	/** Specify the distance from underflow or overflow that is considered "close" */
	uint32_t close_distance;

	/** Ticks per revolution */
	uint32_t ticks_per_revolution;
};


/**
 * @brief Set the encoder variables so the count and overflows are zero
 */
int Encoder16Reset(struct Encoder16Handle *handle);

int Encoder16Update(struct Encoder16Handle *handle);

void Encoder16PrintStatus(struct Encoder16Handle handle);


# endif // __ENCODERS_H