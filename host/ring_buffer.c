/* 
	Pre-allocates RING_BUFFER_SIZE frames for `receiver.c` to write into (Thread 1), 
	`pipeline_controller.cpp` to process, and `gl_display.cpp` to read from (Thread 2),
	in that order. All frame memory is stored here and freed at shutdown.

	Reader chases writer in a loop that periodically circles back to slot 0 in a 
	simple, lock-free single-producer single-consumer (SPSC) pattern. 
*/
#include <stdbool.h>
#include <stdatomic.h>

#include "sonopsis/frame.h"
#include "sonopsis/pipeline_config.h"

static const int RING_BUFFER_SIZE = 4;

typedef struct {
	struct DataFrame frames[RING_BUFFER_SIZE];
	float data_pool[RING_BUFFER_SIZE][NUM_CHANNELS*NUM_SAMPLES_PER_LINE];
	_Atomic int write_idx;
	_Atomic int read_idx;
} RingBuffer;

void initialize(RingBuffer* rb) {
	for (int i = 0; i < RING_BUFFER_SIZE; i++) {
		rb->frames[i].sample_data = rb->data_pool[i];
	}
}

bool write_to_buffer(RingBuffer* rb, struct DataFrame* df) {
	if (is_full(rb)) return false;
	
	int idx = atomic_load(&rb->write_idx);
	
	// Copy frame header into slot
	rb->frames[idx].timestamp = df->timestamp;
	rb->frames[idx].channel_count = df->channel_count;
	rb->frames[idx].samples_per_line = df->samples_per_line;

	// Copy all float data into slot in bytes
	memcpy(rb->data_pool[idx], df->sample_data, 
		df->channel_count * df->samples_per_line * sizeof(float)); // 1 float = 32 bits = 4 bytes

	// Advance write_idx
	atomic_store(&rb->write_idx, (idx + 1) % RING_BUFFER_SIZE);
	return true;
}

bool read_buffer(RingBuffer* rb, struct DataFrame** df) {
	if (is_empty(rb)) return false;

	int idx = atomic_load(&rb->read_idx);
	// Dereference caller's DataFrame*, then have it point to slot idx
	*df = &rb->frames[idx]; 

	// Advance read index
	atomic_store(&rb->read_idx, (idx + 1) % RING_BUFFER_SIZE);
	return true;
}

bool is_full(RingBuffer* rb) {
	// "Full" when write_idx is a slot behind read_idx
	return (rb->write_idx + 1) % RING_BUFFER_SIZE == rb->read_idx; 
}

bool is_empty(RingBuffer* rb) {
	return rb->write_idx == rb->read_idx;
}