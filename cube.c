#include <gb/gb.h>
#include <gb/drawing.h>

#include "tables.h"

#define WX GRAPHICS_WIDTH
#define WY GRAPHICS_HEIGHT
#define HX (WX >> 1)
#define HY (WY >> 1)

typedef struct
{
  INT8 x;
  INT8 y;
  INT8 z;
} vertex_t;

typedef struct
{
  UINT8 i;
  UINT8 j;
} edge_t;

/* cube indices

      7           6
       +---------+
      /|        /|
   3 / |     2 / |
    +---------+  |
    |  |      |  |
    |  +------|--+
    | / 4     | / 5
    |/        |/
    +---------+
   0           1
*/

vertex_t vertices[8] = {
  { -40, -40,  40 },
  {  40, -40,  40 },
  {  40,  40,  40 },
  { -40,  40,  40 },
  { -40, -40, -40 },
  {  40, -40, -40 },
  {  40,  40, -40 },
  { -40,  40, -40 },
};
vertex_t vertices_out[8];

edge_t edges[12] = {
  { 0, 1 }, { 0, 3 }, { 0, 4 },
  { 1, 2 }, { 1, 5 }, { 2, 3 },
  { 2, 6 }, { 3, 7 }, { 4, 5 },
  { 4, 7 }, { 5, 6 }, { 6, 7 },
};

void main(void)
{
  UINT8 i = 0;
  UINT8 a = 0;
  INT8 s8 = 0;
  INT8 c8 = 0;

  disable_interrupts();
  cpu_fast();// GBC
  enable_interrupts();

  color(BLACK, WHITE, SOLID);

  while (1)
  {
    // step
    s8 = sin8[a];
    c8 = cos8[a];
    a++;

    // transform and project
    for (i = 0; i != 8; i++)
    {
      vertex_t * v = vertices + i;
      vertex_t * v_out = vertices_out + i;

      INT16 x0 = v->x;
      INT16 y0 = v->y;
      INT16 x1 = x0 * c8 - y0 * s8;
      INT16 y1 = y0 * c8 + x0 * s8;

      //TODO projection

      // sign 0x8000
      // rest 0x7fff
      v_out->x = ((x1 & 0x7fff) >> 7) | ((x1 & 0x8000) >> 8);
      v_out->y = ((y1 & 0x7fff) >> 7) | ((y1 & 0x8000) >> 8);
    }

    // draw wireframe
    for (i = 0; i != 12; i++)
    {
      edge_t * edge = edges + i;
      vertex_t * v0 = vertices_out + edge->i;
      vertex_t * v1 = vertices_out + edge->j;

      // gprintf("(%d,%d) -> (%d,%d)\n", v0->x, v0->y, v1->x, v1->y);
      if (v0->x > v1->x)
        line(HX + v1->x, HY + v1->y, HX + v0->x, HY + v0->y);
      else
        line(HX + v0->x, HY + v0->y, HX + v1->x, HY + v1->y);
    }

    // sync
    //TODO
  }
}