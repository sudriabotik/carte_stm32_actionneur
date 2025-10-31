# include "encoders.h"


uint32_t GetEncoderSituation(struct Encoder16Handle *handle)
{
	if (*handle->tim_counter < handle->close_distance) return ENCODER_SITUATION_UNDERFLOW_CLOSE;
	if (*handle->tim_counter > 0xFFFF - handle->close_distance) return ENCODER_SITUATION_OVERFLOW_CLOSE;
	return ENCODER_SITUATION_NOTHING;
}

int Encoder16ResetMiddle(struct Encoder16Handle *handle)
{
	if (handle == 0) return -1;

	handle->rollover_count = 125; // about half of the maximum amout of rollovers
	*handle->tim_counter = 0; // set the timer counter to 0
	
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

	uint32_t prev_count = handle->total_count;
	handle->total_count = *handle->tim_counter + 0xFFFF * handle->rollover_count;
	handle->total_count_delta = handle->total_count - prev_count;
	return 0;
}