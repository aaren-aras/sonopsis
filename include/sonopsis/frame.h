// frame.h

#pragma once
#include <stdint.h>
#include "pipeline_config.h"

//struct FrameHeader {
//	uint64_t timestamp; 
//	int32_t channel_count;
//	int32_t samples_per_line;
//};
//
//struct DataFrame {
//	FrameHeader header;
//	float* sample_data;
//};

struct DataFrame {
	uint64_t timestamp; // nanoseconds since epoch (January 1 1970, 00:00:00 UTC)
	int32_t channel_count;
	int32_t samples_per_line;
	// sample_data[channel * samples_per_line + sample_index]
	//float sample_data[Constants::NUM_CHANNELS * Constants::NUM_SAMPLES_PER_LINE]; 
	float* sample_data;
};