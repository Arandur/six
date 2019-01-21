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

  hex::board board;

  // Initial setup
  board.set_color({0, 1}, hex::color::RED);
  board.set_color({1, 0}, hex::color::BLACK);

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
