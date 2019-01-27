#include "board.h"
#include "draw.h"
#include "sdlmm.h"

#include <cstddef>
#include <iostream>
#include <memory>

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "Could not initialize SDL: " << SDL_GetError() << std::endl;
    return EXIT_FAILURE;
  }

  sdl::Window   win("Six", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
  sdl::Renderer ren(win, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  sdl::Texture  tex(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 640, 480);

  auto board = hex::board_t::init();

  board.add({ 0, 3 }, hex::color_t::RED);

  while (true)
  {
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        return EXIT_SUCCESS;
      }
    }

    draw_board(board, tex);

    ren.clear();
    ren.copy(tex);
    ren.present();
  }
}
