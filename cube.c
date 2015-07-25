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
  { -15, -15,  15 },
  {  15, -15,  15 },
  {  15,  15,  15 },
  { -15,  15,  15 },
  { -15, -15, -15 },
  {  15, -15, -15 },
  {  15,  15, -15 },
  { -15,  15, -15 },
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
  UINT8 x = 0;
  UINT8 y = 0;

  disable_interrupts();
  cpu_fast();// GBC
  enable_interrupts();

  color(BLACK, WHITE, SOLID);

  while (1)
  {
    UINT8 i = 0;

    // debug
    x = (x + 1) % WX;
    y = (y + 1) % WY;
    plot_point(x, y);

    // transform
    for (i = 0; i != 8; i++)
    {
      vertex_t * v = vertices + i;
      vertex_t * v_out = vertices_out + i;      
      //TODO
      v_out->x = v->x;
      v_out->y = v->y;
    }

    // plot wireframe
    for (i = 0; i != 12; i++)
    {
      edge_t * edge = edges + i;
      vertex_t * vi = vertices_out + edge->i;
      vertex_t * vj = vertices_out + edge->j;
      line(HX + vi->x, HY + vi->y, HX + vj->x, HY + vj->y);
    }
  }
}