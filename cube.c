#include <gb/gb.h>
#include <gb/cgb.h>
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
  UINT8 x;
  UINT8 y;
} pixel_t;

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

pixel_t pixels[8];

edge_t edges[12] = {
  { 0, 1 }, { 0, 3 }, { 0, 4 },
  { 1, 2 }, { 1, 5 }, { 2, 3 },
  { 2, 6 }, { 3, 7 }, { 4, 5 },
  { 4, 7 }, { 5, 6 }, { 6, 7 },
};

UINT8 heart[81] = {
  0,1,1,1,0,1,1,1,0,
  1,1,0,1,1,1,1,1,1,
  1,0,1,1,1,1,1,1,1,
  1,0,1,1,1,1,1,1,1,
  0,1,1,1,1,1,1,1,0,
  0,1,1,1,1,1,1,0,0,
  0,0,1,1,1,1,0,0,0,
  0,0,1,1,0,0,0,0,0,
  0,0,1,0,0,0,0,0,0,
};

UWORD palette[] =
{
  RGB_BLACK, RGB_WHITE, RGB_YELLOW, RGB_RED,
};

void plot_heart(UINT8 x, UINT8 y)
{
  UINT8 ex = x + 9;
  UINT8 ey = y + 9;
  UINT8 ix, iy, k = 0;
  for (iy = y; iy != ey; iy++)
  {
    for (ix = x; ix != ex; ix++)
    {
      if (heart[k++] != 0)
        plot_point(ix, iy);
    }
  }
}

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

  mode(M_DRAWING);
  color(3, 0, SOLID);

  set_bkg_palette(0, 1, palette);

  plot_heart(12, 30);
  plot_heart(22, 26);

  for (i = 0; i != 8; i++)
  {
    pixels[i].x = HX;
    pixels[i].y = HY;
  }

  while (1)
  {
    INT8 xmin = pixels[0].x;
    INT8 xmax = pixels[0].x;
    INT8 ymin = pixels[0].y;
    INT8 ymax = pixels[0].y;

    // calc min-max of last drawn cube
    for (i = 1; i != 8; i++)
    {
      pixel_t * p = pixels + i;
      if (xmin > p->x)
          xmin = p->x;
      else if (xmax < p->x)
          xmax = p->x;
      if (ymin < p->y)
          ymin = p->y;
      else if (ymax > p->y)
          ymax = p->y;
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
      pixel_t * p = pixels + i;

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

      p->x = HX + (x < 0 ? -dx : dx);
      p->y = HY + (y < 0 ? -dy : dy);
    }

    // sync
    //wait_vbl_done();

    // draw wireframe
    color(WHITE, WHITE, SOLID);
    box(xmin - 1, ymin + 1, xmax + 1, ymax - 1, M_FILL);

    color(1, 0, SOLID);
    for (i = 0; i != 12; i++)
    {
      edge_t * edge = edges + i;
      pixel_t * p0 = pixels + edge->i;
      pixel_t * p1 = pixels + edge->j;

      if (p0->x > p1->x)
        line(p1->x, p1->y, p0->x, p0->y);
      else
        line(p0->x, p0->y, p1->x, p1->y);
    }
  }
}
