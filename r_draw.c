#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gamedef.h"
#include "r_draw.h"

void R_DrawChecks(uint32_t *buf, uint32_t rgba1, uint32_t rgba2)
{
   uint32_t *line   = buf;

   for (unsigned y = 0; y < FRAME_BUFFER_SIZE / WINDOW_WIDTH; y++, line += WINDOW_WIDTH)
   {
      unsigned index_y = (y >> 4) & 1;
      for (unsigned x = 0; x < WINDOW_WIDTH; x++)
      {
         unsigned index_x = (x >> 4) & 1;
         line[x] = (index_y ^ index_x) ? rgba1 : rgba2;
      }
   }
}

void R_DrawQuarters(uint32_t *buf, uint32_t rgba1, uint32_t rgba2)
{
   uint32_t *line   = buf;

   for (unsigned y = 0; y < FRAME_BUFFER_SIZE / WINDOW_WIDTH; y++, line += WINDOW_WIDTH)
   {
      unsigned index_y = (WINDOW_HEIGHT >> 1) > y;
      for (unsigned x = 0; x < WINDOW_WIDTH; x++)
      {
         unsigned index_x = (WINDOW_WIDTH >> 1) > x;
         line[x] = (index_y ^ index_x) ? rgba1 : rgba2;
      }
   }
}

void R_DrawPoint(uint32_t *buf, int x, int y, uint32_t rgba)
{
   int stride = WINDOW_WIDTH;
   int pixels = WINDOW_WIDTH * WINDOW_HEIGHT;

   if (y * stride + x < pixels && y * stride + x >= 0)
      buf[y * stride + x] = rgba;
}

void R_DrawLine(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba)
{
   int stride = WINDOW_WIDTH;
   int pixels = WINDOW_WIDTH * WINDOW_HEIGHT;
   int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
   int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
   int err = dx + dy, e2;

   for(;;)
   {
      if (y0 * stride + x0 < pixels && y0 * stride + x0 >= 0)
         buf[y0 * stride + x0] = rgba;
      if (x0 == x1 && y0 == y1)
         break;
      e2 = 2 * err;
      if (e2 >= dy)
      {
         err += dy;
         x0 += sx;
      }
      if (e2 <= dx)
      {
         err += dx;
         y0 += sy;
      }
   }
}

void R_DrawOval(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba)
{
   int a = abs(x1 - x0);
   int b = abs(y1 - y0);
   int b1 = b & 1;
   long dx = 4 * (1 - a) * b * b;
   long dy = 4 * (b1 + 1) * a * a;
   long err = dx + dy + b1 * a * a;
   long e2;

   if (x0 > x1)
   {
      x0 = x1;
      x1 += a;
   }
   if (y0 > y1)
      y0 = y1;
   y0 += (b + 1) / 2;
   y1 = y0 - b1;
   a *= 8 * a;
   b1 = 8 * b * b;

   do
   {
      R_DrawPoint(buf, x1, y0, rgba);
      R_DrawPoint(buf, x0, y0, rgba);
      R_DrawPoint(buf, x0, y1, rgba);
      R_DrawPoint(buf, x1, y1, rgba);
      e2 = 2 * err;
      if (e2 <= dy)
      {
         y0 ++;
         y1 --;
         err += dy += a;
      }
      if (e2 >= dx || 2 * err > dy)
      {
         x0 ++;
         x1 --;
         err += dx += b1;
      }
   } while (x0 <= x1);

   while (y0 - y1 < b)
   {
      R_DrawPoint(buf, x0 - 1, y0, rgba);
      R_DrawPoint(buf, x1 + 1, y0 ++, rgba);
      R_DrawPoint(buf, x0 - 1, y1, rgba);
      R_DrawPoint(buf, x1 + 1, y1 --, rgba);
   }
}

void R_FillOval(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba)
{
   int a = abs(x1 - x0);
   int b = abs(y1 - y0);
   int b1 = b & 1;
   long dx = 4 * (1 - a) * b * b;
   long dy = 4 * (b1 + 1) * a * a;
   long err = dx + dy + b1 * a * a;
   long e2;

   if (x0 > x1)
   {
      x0 = x1;
      x1 += a;
   }
   if (y0 > y1)
      y0 = y1;
   y0 += (b + 1) / 2;
   y1 = y0 - b1;
   a *= 8 * a;
   b1 = 8 * b * b;

   do
   {
      R_DrawLine(buf, x1, y0, x0, y0, rgba);
      R_DrawLine(buf, x1, y1, x0, y1, rgba);
      e2 = 2 * err;
      if (e2 <= dy)
      {
         y0 ++;
         y1 --;
         err += dy += a;
      }
      if (e2 >= dx || 2 * err > dy)
      {
         x0 ++;
         x1 --;
         err += dx += b1;
      }
   } while (x0 <= x1);

   while (y0 - y1 < b)
   {
      R_DrawPoint(buf, x0 - 1, y0, rgba);
      R_DrawPoint(buf, x1 + 1, y0 ++, rgba);
      R_DrawPoint(buf, x0 - 1, y1, rgba);
      R_DrawPoint(buf, x1 + 1, y1 --, rgba);
   }
}

void R_FillRect(uint32_t *buf, int x0, int y0, int x1, int y1, uint32_t rgba)
{
   int stride = WINDOW_WIDTH;
   int pixels = WINDOW_WIDTH * WINDOW_HEIGHT;

   for (unsigned y = y0; y < y1; y++)
      for (unsigned x = x0; x < x1; x++)
         if (y * stride + x < pixels && y * stride + x >= 0)
            buf[y * stride + x] = rgba;
}

/* EOF */
