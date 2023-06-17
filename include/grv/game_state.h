#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "grv/base_defines.h"
#include "grv/frame_buffer.h"

typedef struct {
    frame_buffer_t frame_buffer;
    bool is_running;
} game_state_t;


void game_state_init(game_state_t* game_state);

#endif