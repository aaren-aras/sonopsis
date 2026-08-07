// pipeline_config.h
#pragma once // include file once per compilation

//struct ImageDims {
//	int height, width;
//};

/* Only one instance of each across entire program */

// Macros work everywhere (C/C++/CUDA)
#define SAMPLE_RATE 40000000 // 40 MHz
#define NUM_CHANNELS 8
#define SPEED_OF_SOUND 1540.0f // m/s, in tissue
#define IMAGING_DEPTH 0.05f // metres
//#define ImageDims OutputImageDims{500, 500}
#define OUTPUT_IMAGE_HEIGHT 500
#define OUTPUT_IMAGE_WIDTH 500

#define NUM_RF_LINES 64

#define ROUNDTRIP_TIME (2.0f * IMAGING_DEPTH) / (SPEED_OF_SOUND) // seconds
#define NUM_SAMPLES_PER_LINE ((int)(ROUNDTRIP_TIME * SAMPLE_RATE)) 


