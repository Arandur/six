#include "draw.h"

#include <cairo.h>

void draw_board(const hex::board& /*board*/, sdl::Texture& tex)
{
  auto lock = tex.lock();

  cairo_surface_t *surface = cairo_image_surface_create_for_data(
      static_cast<unsigned char*>(lock.pixels), CAIRO_FORMAT_ARGB32, 640, 480, lock.pitch);
  cairo_t *cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  cairo_set_source_rgb(cr, 1, 0, 0);
  cairo_rectangle(cr, 100, 100, 100, 100);
  cairo_fill(cr);

  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}
