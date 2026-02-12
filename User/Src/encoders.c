# include "encoders.h"

# include <inttypes.h>
# include <stdio.h>


uint32_t GetEncoderSituation(struct Encoder16Handle *handle)
{
	if (*handle->tim_counter < handle->close_distance) return ENCODER_SITUATION_UNDERFLOW_CLOSE;
	if (*handle->tim_counter > 0xFFFF - handle->close_distance) return ENCODER_SITUATION_OVERFLOW_CLOSE;
	return ENCODER_SITUATION_NOTHING;
}

int Encoder16Reset(struct Encoder16Handle *handle)
{
	if (handle == 0) return -1;

	handle->rollover_count = 0; // set the rollover count to 0
	handle->total_count = 0;
	handle->total_count_delta = 0;
	*handle->tim_counter = 0; // set the timer counter to 0
	handle->situation = GetEncoderSituation(handle);
	
	Encoder16Update(handle); // update the calculated values

	return 0;
}

int Encoder16Update(struct Encoder16Handle *handle)
{
	if (handle == 0) return -1;

	uint32_t current_situation = GetEncoderSituation(handle);

	if (handle->situation == ENCODER_SITUATION_OVERFLOW_CLOSE && current_situation == ENCODER_SITUATION_UNDERFLOW_CLOSE)
	{
		handle->rollover_count ++;
	}
	if (handle->situation == ENCODER_SITUATION_UNDERFLOW_CLOSE && current_situation == ENCODER_SITUATION_OVERFLOW_CLOSE)
	{
		handle->rollover_count --;
	}

	handle->situation = current_situation;

	int32_t prev_count = handle->total_count;
	handle->total_count = (int32_t)*handle->tim_counter + 0xFFFF * handle->rollover_count;

	handle->total_count_delta = handle->total_count - prev_count;
	return 0;
}

void Encoder16PrintStatus(struct Encoder16Handle handle)
{
	printf("processed absolute position : %"PRIi32"\n", handle.total_count);
	printf("position delta : %"PRIi32"\n", handle.total_count_delta);
	printf("rollover count : %"PRIi32"\n", handle.rollover_count);
	printf("situation : %"PRIu32"\n", handle.situation);
	printf("counter register raw : %"PRIu32"\n", TIM3->CNT);
}
