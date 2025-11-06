/**
 * @file recorder.h
 * @author sudriabotik
 * @brief buffers to store control loops results (target, drive, and actual)
 */

# ifndef __RECORDER_H
# define __RECORDER_H

# include "stm32g4xx_hal.h"
# include "stdio.h"

// how many slots in the array, to store 
# define RECORDING_LENGTH_TICKS 100U
# define NUM_RECORDING_TRACKS 1U

struct RecordedTick
{
	uint32_t target;
	uint32_t drive;
	uint32_t actual;
};

/** An array of recording tracks, which contains recording ticks */
extern struct RecordedTick recording[NUM_RECORDING_TRACKS][RECORDING_LENGTH_TICKS];
extern uint32_t recording_timestamp[NUM_RECORDING_TRACKS];


void recorder_append(uint32_t track, struct RecordedTick value);

/**
 * @brief outputs with printf all the recorded data in the track
 */
void recorder_print_recorded_data(uint32_t track);


int recorder_is_track_full(uint32_t track);


# endif //__RECORDER_H