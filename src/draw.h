#pragma once

#include "board.h"
#include "sdlmm.h"

void draw_board(const hex::board_t& board, sdl::Texture& tex);
