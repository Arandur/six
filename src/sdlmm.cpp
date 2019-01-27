#include "sdlmm.h"

namespace sdl
{
void Renderer::clear()
{
  SDL_RenderClear(get());
}

void Renderer::copy(Texture& tex)
{
  SDL_RenderCopy(get(), tex.get(), nullptr, nullptr);
}

void Renderer::present()
{
  SDL_RenderPresent(get());
}

Texture::Lock::Lock(SDL_Texture* tex) :
  _tex(tex)
{
  SDL_LockTexture(_tex, nullptr, &pixels, &pitch);
}

Texture::Lock::~Lock()
{
  SDL_UnlockTexture(_tex);
}

auto Texture::lock() -> Lock
{
  return Lock(get());
}

auto Texture::size() -> Rect
{
  Rect rect { 0, 0, 0, 0 };

  SDL_QueryTexture(get(), nullptr, nullptr, &rect.w, &rect.h);

  return rect;
}
}
