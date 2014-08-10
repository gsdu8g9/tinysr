// TinySR

#ifndef TinySR_HEADER_H
#define TinySR_HEADER_H

#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define FFT_LENGTH 512
#define FRAME_LENGTH 400
#define SHIFT_INTERVAL 160

typedef int64_t samp_t;

// Generic singly linked list based stack.
typedef struct _list_node_t {
	void* datum;
	struct _list_node_t* prev;
	struct _list_node_t* next;
} list_node_t;

// Simply set each field to zero initially to make an empty list.
// Thus, list_t l = {0}; suffices for initializiation.
typedef struct {
	int length;
	list_node_t* head;
	list_node_t* tail;
} list_t;

void list_push(list_t* list, void* datum);
void* list_pop(list_t* list);

// TinySR context, and associated functions.
typedef struct {
	// Public configuration:
	int input_framerate;

	// Private:
	float offset_comp_prev_in;
	float offset_comp_prev_out;
	float* input_buffer;
	int input_buffer_next;
	int input_buffer_samps;
	float* temp_buffer;
	// Feature vector list.
	list_t fv_list;
} tinysr_ctx_t;

typedef struct {
	float log_energy;
	float cepstrum[13];
} feature_vector_t;

// Call to get/free a context.
tinysr_ctx_t* tinysr_allocate_context(void);
void tinysr_free_context(tinysr_ctx_t* ctx);

// Call to pass input samples.
void tinysr_feed_input(tinysr_ctx_t* ctx, samp_t* samples, int length);

// Call to trigger the big expensive recognition operation.
void tinysr_recognize_frames(tinysr_ctx_t* ctx);

// Private functions.
// Initiates a feature extraction run on the contents of input_buffer.
// This function is called automatically by tinysr_feed_input whenever the
// buffer is full, so you should never have to call it yourself.
void tinysr_process_frame(tinysr_ctx_t* ctx);

// This function is used internally for the FFT computation.
void tinysr_fft_dit(float* in_real, float* in_imag, int length, int stride, float* out_real, float* out_imag);
void tinysr_abs_fft(float* array, int length);

#endif
