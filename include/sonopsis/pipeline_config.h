// pipeline_config.h

#pragma once // include file once per compilation

struct ImageDims {
	int height, width;
};

namespace Constants {
	/* 1 unique instance of each across entire program */
	inline constexpr int SAMPLE_RATE = 40'000'000; // Hz
	inline constexpr int NUM_CHANNELS = 8;
	inline constexpr float SPEED_OF_SOUND = 1540.0f; // m/s, in tissue
	inline constexpr float IMAGING_DEPTH = 0.05f; // metres
	inline constexpr ImageDims OutputImageDims{500, 500};
	inline constexpr int NUM_RF_LINES = 64;

	inline constexpr float ROUNDTRIP_TIME = (2.0f * IMAGING_DEPTH) / (SPEED_OF_SOUND); // seconds
	inline constexpr int NUM_SAMPLES_PER_LINE = static_cast<int>(ROUNDTRIP_TIME * SAMPLE_RATE); 

}

