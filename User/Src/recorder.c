# include "recorder.h"

# include <inttypes.h>

# include "formatted_debug_msg.h"

struct RecordedTick recording[NUM_RECORDING_TRACKS][RECORDING_LENGTH_TICKS];
uint32_t recording_timestamp[NUM_RECORDING_TRACKS] = {0};


void recorder_append(uint32_t track, struct RecordedTick value)
{
	if (track >= NUM_RECORDING_TRACKS)
	{
		DEBUG_MSG_ERROR_VAL("RECORDER", "track number %"PRIu32" is too high, doesn't exist", track)
	}

	if (recording_timestamp[track] >= RECORDING_LENGTH_TICKS || recording_timestamp[track] == 0xFFFFU)
	{
		DEBUG_MSG_ERROR_VAL("RECORDER", "cannot record, exceeded track length %"PRIu32"", RECORDING_LENGTH_TICKS)
	}

	recording[track][recording_timestamp[track]] = value;
	recording_timestamp[track] ++;
}


void print_recorded_data(uint32_t track)
{
	if (track >= NUM_RECORDING_TRACKS)
	{
		DEBUG_MSG_ERROR_VAL("RECORDER", "track number %"PRIu32" is too high, doesn't exist", track)
	}

	printf("data_track_%"PRIu32"\n", track);

	for (uint32_t i = 0; i < recording_timestamp[track]; i++)
	{
		printf("%"PRIu32",", recording[track][i].target);
		printf("%"PRIu32",", recording[track][i].drive);
		printf("%"PRIu32";\n", recording[track][i].actual);
	}
	
	printf("data_stop");
}