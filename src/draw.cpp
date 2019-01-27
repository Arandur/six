#include "draw.h"

#include <cairo.h>

#include <cstdio>

static void draw_hex(
    cairo_t* cr, const hex::piece_t& piece,
    double scale, const sdl::Rect& size)
{
  if (piece.color == hex::color_t::RED)
  {
    cairo_set_source_rgb(cr, 1, 0, 0);
  }
  else
  {
    cairo_set_source_rgb(cr, 0, 0, 0);
  }

  const auto cx = piece.coord.x * scale + size.w / 2;
  const auto cy = piece.coord.y * scale + size.h / 2;

  cairo_move_to(cr, cx            , cy + 2 * scale);
  cairo_line_to(cr, cx + 2 * scale, cy +     scale);
  cairo_line_to(cr, cx + 2 * scale, cy -     scale);
  cairo_line_to(cr, cx            , cy - 2 * scale);
  cairo_line_to(cr, cx - 2 * scale, cy -     scale);
  cairo_line_to(cr, cx - 2 * scale, cy +     scale);
  cairo_close_path(cr);
  cairo_fill(cr);
}

void draw_board(const hex::board_t& board, sdl::Texture& tex)
{
  const auto lock = tex.lock();
  const auto size = tex.size();

  cairo_surface_t *surface = cairo_image_surface_create_for_data(
      static_cast<unsigned char*>(lock.pixels), CAIRO_FORMAT_ARGB32, 
      size.w, size.h, lock.pitch);
  cairo_t *cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);

  auto min_x = std::numeric_limits<int16_t>::max();
  auto max_x = std::numeric_limits<int16_t>::min();
  auto min_y = std::numeric_limits<int16_t>::max();
  auto max_y = std::numeric_limits<int16_t>::min();

  for (const auto& piece : board)
  {
    min_x = std::min(min_x, piece.coord.x);
    max_x = std::max(max_x, piece.coord.x);
    min_y = std::min(min_y, piece.coord.y);
    max_y = std::max(max_y, piece.coord.y);
  }

  min_x -= 8;
  max_x += 8;
  min_y -= 8;
  max_y += 8;

  const auto x_scale = size.w / static_cast<double>(max_x - min_x);
  const auto y_scale = size.h / static_cast<double>(max_y - min_y);
  const auto scale   = std::min(x_scale, y_scale);

  for (const auto& piece : board)
  {
    draw_hex(cr, piece, scale, size);
  }

  cairo_destroy(cr);
  cairo_surface_destroy(surface);
}
