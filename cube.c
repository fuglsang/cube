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
  { -1, -1,  1 },
  {  1, -1,  1 },
  {  1,  1,  1 },
  { -1,  1,  1 },
  { -1, -1, -1 },
  {  1, -1, -1 },
  {  1,  1, -1 },
  { -1,  1, -1 },
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
  INT8 s6 = 0;
  INT8 c6 = 0;
  INT8 dd = 0;

  disable_interrupts();
  cpu_fast();// GBC
  enable_interrupts();

  color(BLACK, WHITE, SOLID);

  while (1)
  {
    // draw wireframe
    color(WHITE, BLACK, SOLID);
    for (i = 0; i != 12; i++)
    {
      edge_t * edge = edges + i;
      vertex_t * v0 = vertices_out + edge->i;
      vertex_t * v1 = vertices_out + edge->j;

      if (v0->x > v1->x)
        line(v1->x, v1->y, v0->x, v0->y);
      else
        line(v0->x, v0->y, v1->x, v1->y);
    }
    
    // step
    s6 = sin6[a];
    c6 = cos6[a];
    dd = s6 + s6;
    a += 3;
    
    // transform and project
    for (i = 0; i != 8; i++)
    {
      vertex_t * v = vertices + i;
      vertex_t * v_out = vertices_out + i;

      INT8 x = (v->x * c6) + (v->z * s6) + dd;
      INT8 y = (v->y * 31) + dd;
      INT8 z = (v->z * c6) - (v->x * s6);
      
      /* rotates in z
      INT8 x = (v->x * c6) - (v->y * s6);
      INT8 y = (v->y * c6) + (v->x * s6);
      INT8 z = (v->z * 31);
      */
      
      UINT16 dx = x < 0 ? -x : x;
      UINT16 dy = y < 0 ? -y : y;
      UINT16 dz = 128 - z;
      
      dx = (dx << 5) / dz;
      dy = (dy << 5) / dz;
      
      v_out->x = HX + (x < 0 ? -dx : dx);
      v_out->y = HY + (y < 0 ? -dy : dy);
    }

    // draw wireframe
    color(BLACK, WHITE, SOLID);
    for (i = 0; i != 12; i++)
    {
      edge_t * edge = edges + i;
      vertex_t * v0 = vertices_out + edge->i;
      vertex_t * v1 = vertices_out + edge->j;

      if (v0->x > v1->x)
        line(v1->x, v1->y, v0->x, v0->y);
      else
        line(v0->x, v0->y, v1->x, v1->y);
    }

    // sync
    //TODO
  }
}