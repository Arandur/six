#pragma once

/**
 * RAII containers for SDL objects
 */

#include <SDL2/SDL.h>

#include <memory>
#include <stdexcept>

namespace sdl
{
class Exception : public std::exception
{
public:
  Exception(const char* msg) :
    _msg(strdup(msg))
  {}

  auto what() const noexcept -> const char* override
  {
    return _msg;
  }

private:
  const char* _msg;
};

using Rect = SDL_Rect;

class Window
{
public:
  Window(const char* name, int x_pos, int y_pos, int width, int height, int flags) :
    _win(SDL_CreateWindow(name, x_pos, y_pos, width, height, flags),
         SDL_DestroyWindow)
  {
    if (!_win)
    {
      throw Exception(SDL_GetError());
    }
  }

  auto get() noexcept -> SDL_Window*
  {
    return _win.get();
  }

private:
  std::unique_ptr<SDL_Window, void (&) (SDL_Window*)> _win;
};

class Texture;

class Renderer
{
public:
  Renderer(Window& win, int flags) :
    _ren(SDL_CreateRenderer(win.get(), -1, flags), SDL_DestroyRenderer)
  {
    if (!_ren)
    {
      throw Exception(SDL_GetError());
    }
  }

  auto get() noexcept -> SDL_Renderer*
  {
    return _ren.get();
  }

  void clear();
  void copy(Texture&);
  void present();

private:
  std::unique_ptr<SDL_Renderer, void (&) (SDL_Renderer*)> _ren;
};

class Texture
{
public:
  struct Lock
  {
    Lock(SDL_Texture* tex);
    ~Lock();

    void* pixels;
    int pitch;

  private:
    SDL_Texture* _tex;
  };

  Texture(Renderer& ren, int format, int access, int width, int height) :
    _tex(SDL_CreateTexture(ren.get(), format, access, width, height),
         SDL_DestroyTexture)
  {
    if (!_tex)
    {
      throw Exception(SDL_GetError());
    }
  }

  auto get() -> SDL_Texture*
  {
    return _tex.get();
  }

  auto lock() -> Lock;

  auto size() -> Rect;

private:
  std::unique_ptr<SDL_Texture, void (&) (SDL_Texture*)> _tex;
};
}
