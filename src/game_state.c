#include "game_state.h"

void game_state_init(game_state_t* game_state) {
    frame_buffer_init(&game_state->frame_buffer, FRAME_BUFFER_INDEXED, 256, 256);
    game_state->is_running = true;
}